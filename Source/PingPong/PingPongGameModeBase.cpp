// Copyright Epic Games, Inc. All Rights Reserved.


#include "PingPongGameModeBase.h"

#include "PingPongGoal.h"
#include "PingPongPlayerController.h"
#include "PingPongPlayerPawn.h"
#include "EngineUtils.h"
#include "PingPongBall.h"
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

	UWorld* World { GetWorld() };
	if(World && (!Player1Start || !Player2Start))
	{
		TArray<APlayerStart*> FoundActors;
		utils::FindAllActors<APlayerStart>(World, FoundActors);
		if(FoundActors.Num() > 0)
		{
			Player1Start = FoundActors[0];
		}
		if(FoundActors.Num() > 1)
		{
			Player2Start = FoundActors[1];
		}
	}
	APingPongPlayerController* CurrPlayer;
	APlayerStart* StartPos;
	int32 PlayerID;
	
	if (Player1 == nullptr)
	{
		Player1 = Cast<APingPongPlayerController>(NewPlayer);
		PlayerID = 1;
		CurrPlayer = Player1;
		StartPos = Player1Start;
		UE_LOG(LogTemp, Warning, TEXT("PingPongGameMode: Init player 1"));
	}
	else if (Player2 == nullptr)
	{
		Player2 = Cast<APingPongPlayerController>(NewPlayer);
		PlayerID = 2;
		CurrPlayer = Player2;
		StartPos = Player2Start;
		UE_LOG(LogTemp, Warning, TEXT("PingPongGameMode: Init player 2"));
	}
	else
	{
		UE_LOG(LogTemp, Error,
			TEXT("PingPongGameMode: There are already two players in the game. New connections are not possible"));
		return;
	}
	
	APingPongPlayerPawn* newPawn { Cast<APingPongPlayerPawn>(NewPlayer->GetPawn()) };
	if (!newPawn)
	{
		newPawn = World->SpawnActor<APingPongPlayerPawn>(DefaultPawnClass);
	}

	TArray<APingPongGoal*> FoundGateActors;
	utils::FindAllActors<APingPongGoal>(World, FoundGateActors);
	auto FindGateByPlayerStart = [](const TArray<APingPongGoal*>& Array, const APlayerStart* SearchObject)
	{
		auto Result = Array.FindByPredicate([&](const APingPongGoal *Gate)
		{
			return Gate->PlayerStart == SearchObject;
		});
		return Result != nullptr ? *Result : nullptr;
	};
	
	if (newPawn)
	{
		auto PlayerGate { FindGateByPlayerStart(FoundGateActors, StartPos) }; 
		newPawn->SetActorLocation(StartPos->GetActorLocation());
		newPawn->SetActorRotation(StartPos->GetActorRotation());
		NewPlayer->SetPawn(newPawn);
		CurrPlayer->SetStartTransfrorm(StartPos->GetActorTransform());
		CurrPlayer->Client_InitializeHUD();
		CurrPlayer->Initialize(PlayerID, PlayerGate);

		if (Player1 != nullptr && Player2 != nullptr)
		{
			Player1->Client_SetHUDWindow(EPlayerWindowId::Game);
			Player2->Client_SetHUDWindow(EPlayerWindowId::Game);
			StartGame();
		}
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

bool APingPongGameModeBase::StartGame()
{
	TArray<APingPongBall*> FoundActors;
	utils::FindAllActors<APingPongBall>(GetWorld(), FoundActors);
	if (FoundActors.Num() > 0)
	{
		auto Ball { FoundActors.Last() };
		Ball->StartMove();
		return true;
	}
	return false;
}

