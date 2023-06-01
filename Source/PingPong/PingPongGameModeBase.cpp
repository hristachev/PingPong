// Copyright Epic Games, Inc. All Rights Reserved.


#include "PingPongGameModeBase.h"

#include "PingPongGoal.h"
#include "PingPongPlayerController.h"
#include "PingPongPlayerPawn.h"
#include "EngineUtils.h"
#include "ScoreWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

namespace utils
{
	template<typename T>
	void FindAllActors(UWorld* World, TArray<T*>& Out)
	{
		for (TActorIterator<T> It(World); It; ++It)
		{
			Out.Add(*It);
		}
	}
}

APingPongGameModeBase::APingPongGameModeBase()
{
	DefaultPawnClass = APingPongPlayerPawn::StaticClass();
    PlayerControllerClass = APingPongPlayerController::StaticClass();
}

void APingPongGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void APingPongGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	UWorld* world = GetWorld();
    if(world && (!Player1Start || !Player2Start))
    {
    	TArray<AActor*> foundActors;
    	UGameplayStatics::GetAllActorsOfClass(GetWorld(),
    	APlayerStart::StaticClass(), foundActors);
    	if(foundActors.Num() > 0)
    		Player1Start = (APlayerStart*)foundActors[0];
    	if(foundActors.Num() > 1)
    		Player2Start = (APlayerStart*)foundActors[1];
    }
    APingPongPlayerController * currPlayer;
    APlayerStart* startPos;
	int8 PlayerID;
    if(Player1 == nullptr)
    {
    	Player1 = (APingPongPlayerController*)NewPlayer;
    	PlayerID = 1;
    	currPlayer = Player1;
    	startPos = Player1Start;
    	UE_LOG(LogTemp, Warning, TEXT("PingPongGameMode: Init player 1"));
    }
    else if(Player2 == nullptr)
    {
    	Player2 = (APingPongPlayerController*)NewPlayer;
    	PlayerID = 2;
    	currPlayer = Player2;
    	startPos = Player2Start;
    	UE_LOG(LogTemp, Warning, TEXT("PingPongGameMode: Init player 2"));
    }
    else
    {
		UE_LOG(LogTemp, Error, TEXT("PingPongGameMode: There are already two players in the game. New connections are not possible"));
		return;
    }
    APingPongPlayerPawn* newPawn = Cast<APingPongPlayerPawn>(NewPlayer->GetPawn());
	if(!newPawn)
    {
		newPawn = world->SpawnActor<APingPongPlayerPawn>(DefaultPawnClass);
    }
	TArray<APingPongGoal*> FoundGoalActors;
	utils::FindAllActors<APingPongGoal>(world, FoundGoalActors);
	auto FindGoalByPlayerStart = [](const TArray<APingPongGoal*>& Array, const APlayerStart* SearchObject)
	{
		auto Result = Array.FindByPredicate([&](const APingPongGoal *Goal)
		{
			return Goal->PlayerStart == SearchObject;
		});
		return Result != nullptr ? *Result : nullptr;
	};
	
    if(newPawn)
    {
    	auto PlayerGoal = FindGoalByPlayerStart(FoundGoalActors, startPos);
    	newPawn->SetActorLocation(startPos->GetActorLocation());
    	newPawn->SetActorRotation(startPos->GetActorRotation());
    	NewPlayer->SetPawn(newPawn);
    	currPlayer->SetStartTransfrorm(startPos->GetActorTransform());
    	currPlayer->Client_InitializeHUD();
    	currPlayer->Initialize(PlayerID, PlayerGoal);
    }
    else
    {
		UE_LOG(LogTemp, Error, TEXT("Start position not setted in PingPongGameMode!"));
    }
}

void APingPongGameModeBase::PlayerGoal(int8 PlayerID)
{
	auto CurrentPlayerScore = 0;
	if (PlayerID == 1)
	{
		CurrentPlayerScore = ++ScorePlayer2;
	}
	else if (PlayerID == 2)
	{
		CurrentPlayerScore = ++ScorePlayer1;
	}

	for ( auto It = GetWorld()->GetPlayerControllerIterator(); It ; ++It)
	{
		auto PlayerController = Cast<APingPongPlayerController>(*It);
		if (PlayerController != nullptr)
		{
			if (PlayerController->GetPlayerID() == PlayerID)
			{
				PlayerController->UpdateWidgetEnemyScore(CurrentPlayerScore);
			}
			else
			{
				PlayerController->UpdateWidgetPlayerScore(CurrentPlayerScore);
			}
		}
	}
}

