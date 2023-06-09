// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongBall.h"

#include "DrawDebugHelpers.h"
#include "PingPongGoal.h"
#include "Components/SphereComponent.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystem.h"
#include "Net/UnrealNetwork.h"


namespace utils
{
	template <typename Class, typename ResClass, typename BindRetType = void, typename... BindArgTypes>
	TSharedPtr<FStreamableHandle> AsyncLoad(
		Class* InObject,
		typename TMemFunPtrType<false, Class, BindRetType(BindArgTypes...)>::Type BindFunc,
		TSoftObjectPtr<ResClass> ClassRef)
	{
		FStreamableDelegate LoadMeshDelegate;
		LoadMeshDelegate.BindUObject(InObject, BindFunc);
		UAssetManager& AssetManager { UAssetManager::Get() };
		FStreamableManager& StreamableManager { AssetManager.GetStreamableManager() };
		return StreamableManager.RequestAsyncLoad(ClassRef.ToSoftObjectPath(), LoadMeshDelegate);
	}
}

// Sets default values
APingPongBall::APingPongBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Ball BodyCollider"));
	SetRootComponent(BodyCollision);
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball BodyMesh"));
	BodyMesh->SetupAttachment(RootComponent);
	BodyMesh->SetIsReplicated(true);
	SetReplicates(true);
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void APingPongBall::BeginPlay()
{
	Super::BeginPlay();

	LoadBodyMesh();
	LoadHitEffect();
	
}

void APingPongBall::LoadBodyMesh()
{
	auto AssetHandle { utils::AsyncLoad<APingPongBall, UStaticMesh>(
		this, &APingPongBall::OnBodyMeshLoaded,BodyMeshRef) };
	AssetHandles.Add(AssetHandle);
}

void APingPongBall::OnBodyMeshLoaded()
{
	for (const auto &AssetHandle : AssetHandles)
	{
		if (auto LoadedMesh = Cast<UStaticMesh>(AssetHandle.Get()->GetLoadedAsset()))
		{
			BodyMesh->SetStaticMesh(LoadedMesh);
			if (auto Material = LoadBodyMaterial())
			{
				BodyMesh->SetMaterial(0, Material);
			}
		}
	}
}

UMaterial* APingPongBall::LoadBodyMaterial()
{
	FStreamableManager& StreamableManager { UAssetManager::Get().GetStreamableManager() };
	if (BodyMaterialRef.IsPending())
	{
		const FSoftObjectPath& AssetMaterialRef { BodyMaterialRef.ToSoftObjectPath() };
		BodyMeshRef = Cast<UMaterial>(StreamableManager.LoadSynchronous(AssetMaterialRef));
	}
	return BodyMaterialRef.Get();
}

void APingPongBall::LoadHitEffect()
{
	auto AssetHandle { utils::AsyncLoad<APingPongBall, UParticleSystem>(
		this, &APingPongBall::OnHitEffectLoaded, HitEffectRef) };
	AssetHandles.Add(AssetHandle);
}

void APingPongBall::OnHitEffectLoaded()
{
	for (const auto &AssetHandle : AssetHandles)
	{
		if (auto LoadedHitEffect = Cast<UParticleSystem>(AssetHandle.Get()->GetLoadedAsset()))
		{
			HitEffect = LoadedHitEffect;
		}
	}
}

void APingPongBall::Server_StartMove_Implementation()
{
	IsMoving = true;
}

bool APingPongBall::Server_StartMove_Validate()
{
	return true;
}

void APingPongBall::Server_Move_Implementation(float DeltaTime)
{
	FVector forward = GetActorForwardVector();
	FVector currLoc = GetActorLocation();
	FVector newLoc = currLoc + forward * MoveSpeed * DeltaTime;
	FHitResult hitResult;

	if (!SetActorLocation(newLoc, true, &hitResult))
	{
		UE_LOG(LogTemp, Warning, TEXT("Ball %s Collided with %s"), *GetName(), *hitResult.GetComponent()->GetName());

		if (auto playerGoal = Cast<APingPongGoal>(hitResult.GetActor()))
		{
			if (hitResult.GetComponent()->GetName() == "Goal Collision")
			{
				playerGoal->GoalCheck();
			}
		}
    	
		FVector moveVector = currLoc - newLoc;
		moveVector.Z = 0;
		moveVector.Normalize();
    	
		FVector resetPosition = currLoc + moveVector * DeltaTime * 2 * MoveSpeed;
		DrawDebugDirectionalArrow(
			GetWorld(),
			resetPosition + moveVector * 300,
			resetPosition,
			30,
			FColor::Green,
			true,
			3.f,
			0,
			3
		);
    	
		FVector impactCorrection = hitResult.ImpactPoint + hitResult.ImpactNormal * 300;
		DrawDebugDirectionalArrow(
			GetWorld(),
			hitResult.ImpactPoint,
			impactCorrection,
			30,
			FColor::Blue,
			true,
			3,
			0,
			3
		);
    	
		float AimAtAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(moveVector, hitResult.ImpactNormal)));
		moveVector = moveVector.RotateAngleAxis(AimAtAngle * 2, FVector(0,0,1));
    	
		FVector newTargetMove = newLoc + moveVector * 300;
		DrawDebugDirectionalArrow(
			GetWorld(),
			newLoc,
			newTargetMove,
			30,
			FColor::Red,
			true,
			3.f,
			0,
			3
		);
		SetActorLocation(resetPosition);
		FRotator currRotation = GetActorRotation();
		FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(currLoc, newTargetMove);
		newRotation.Pitch = currRotation.Pitch;
		SetActorRotation(newRotation);
		Multicast_HitEffect();
	}
}

bool APingPongBall::Server_Move_Validate(float DeltaTime)
{
	return true;
}

void APingPongBall::Server_StopMove_Implementation()
{
	IsMoving = false;
}

bool APingPongBall::Server_StopMove_Validate()
{
	return true;
}

void APingPongBall::Multicast_HitEffect_Implementation()
{
	UWorld * world = GetWorld();
    if(world && HitEffect)
    {
   		 UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect,
   		 GetActorLocation());
    }
}

// Called every frame
void APingPongBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetNetMode() != ENetMode::NM_Client)
    {
		Server_Move(DeltaTime);
    }
}

void APingPongBall::StartMove()
{
	Server_StartMove();
}

void APingPongBall::StopMove()
{
	Server_StopMove();
}

void APingPongBall::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APingPongBall, IsMoving);
}

