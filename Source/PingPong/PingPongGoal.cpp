// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongGoal.h"

#include "GameModeInfoCustomizer.h"
#include "PingPongBall.h"
#include "PingPongGameModeBase.h"
#include "Components/BoxComponent.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
APingPongGoal::APingPongGoal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Goal Collision"));
	SetRootComponent(BoxCollision);

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Goal Mesh"));
	BodyMesh->SetupAttachment(RootComponent);
	BodyMesh->SetIsReplicated(true);
	SetReplicates(true);
}

// Called when the game starts or when spawned
void APingPongGoal::BeginPlay()
{
	Super::BeginPlay();
}

void APingPongGoal::Server_GoalCheck_Implementation()
{
	auto GameMode = Cast<APingPongGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode != nullptr)
	{
		GameMode->PlayerGoal(PlayerID);
	}
}

bool APingPongGoal::Server_GoalCheck_Validate()
{
	return true;
}

void APingPongGoal::Server_SetPlayerID_Implementation(int32 ID)
{
	PlayerID = ID;
}

bool APingPongGoal::Server_SetPlayerID_Validate(int32 ID)
{
	return ID !=0;
}

// Called every frame
void APingPongGoal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APingPongGoal::GoalCheck()
{
	Server_GoalCheck();
}

void APingPongGoal::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(APingPongGoal, PlayerID, COND_SimulatedOnly);
}

void APingPongGoal::SetPlayerID(int32 ID)
{
	Server_SetPlayerID(ID);
}

