// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongGoal.h"

#include "GameModeInfoCustomizer.h"
#include "PingPongBall.h"
#include "PingPongGameModeBase.h"
#include "Components/BoxComponent.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APingPongGoal::APingPongGoal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Goal Collision"));
	SetRootComponent(BoxCollision);

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Goal Mesh"));
	BodyMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APingPongGoal::BeginPlay()
{
	Super::BeginPlay();
	
}

void APingPongGoal::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Server_CheckOverlap(OtherActor, false);
}

void APingPongGoal::Server_CheckOverlap_Implementation(AActor* _ActorRef, bool _bOverlapEnd)
{
	APingPongBall* BallActor = Cast<APingPongBall>(_ActorRef);
	APingPongGameModeBase* CurrentGM = Cast<APingPongGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (BallActor && CurrentGM->GetPlayer1())
	{
		CurrentGM->AddScore(1);
	}
	if (BallActor && CurrentGM->GetPlayer2())
	{
		CurrentGM->AddScore(1);
	}
}

// Called every frame
void APingPongGoal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

