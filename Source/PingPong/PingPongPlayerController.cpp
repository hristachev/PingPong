// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongPlayerController.h"

#include "PingPongGoal.h"
#include "PingPongPlatform.h"
#include "ScoreWidget.h"
#include "Net/UnrealNetwork.h"


APingPongPlayerController::APingPongPlayerController()
{
	SetReplicates(true);
}

void APingPongPlayerController::SetStartTransfrorm(FTransform NewStartTransform)
{
	StartTransform = NewStartTransform;
}

void APingPongPlayerController::Client_SetHUDWindow_Implementation(EPlayerWindowId windowId)
{
	if (ScoreWidget)
	{
		ScoreWidget->SwitchWidget(static_cast<int32>(windowId));
	}
}

void APingPongPlayerController::Initialize_Implementation(int32 NewPlayerID, APingPongGoal* NewGoal)
{
	if (Platform)
	{
		Platform->Destroy();
	}
	SpawnPlatform(PlatformClass);

	PlayerID = NewPlayerID;
	PongGoal = NewGoal;
	PongGoal->SetPlayerID(PlayerID);
}

bool APingPongPlayerController::Initialize_Validate(int32 NewPlayerID, APingPongGoal* NewGoal)
{
	return (NewPlayerID != 0 && NewGoal != nullptr);

}

void APingPongPlayerController::Client_InitializeHUD_Implementation()
{
	if (!ScoreWidget)
	{
		ScoreWidget = CreateWidget<UScoreWidget>(this, WidgetClass);
		if (ScoreWidget)
		{
			ScoreWidget->UpdatePlayerScore(0);
			ScoreWidget->UpdateEnemyScore(0);
			ScoreWidget->AddToViewport();
		}
	}
}

bool APingPongPlayerController::Client_InitializeHUD_Validate()
{
	return (WidgetClass != nullptr);
}

void APingPongPlayerController::UpdateWidgetPlayerScore_Implementation(int32 Score)
{
	ScoreWidget->UpdatePlayerScore(Score);
}

void APingPongPlayerController::UpdateWidgetEnemyScore_Implementation(int32 Score)
{
	ScoreWidget->UpdateEnemyScore(Score);
}

void APingPongPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("Move", this, &APingPongPlayerController::MoveRight);
}

void APingPongPlayerController::MoveRight(float AxisValue)
{
	if(AxisValue != 0)
	{
		UE_LOG(LogTemp, Warning,
		TEXT("APingPongPlayerController::MoveRight"));
	}
	Server_PlatformMoveRight(AxisValue);
}

void APingPongPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(APingPongPlayerController, PlayerID, COND_SimulatedOnly);
}

void APingPongPlayerController::Server_PlatformMoveRight_Implementation(float AxisValue)
{
	if(Platform)
	{
		if(AxisValue != 0)
		{
			UE_LOG(LogTemp, Warning,
			TEXT("APingPongPlayerController::Server_PlatformMoveRight_Implementation"));
		}
		Platform->Server_MoveRight(AxisValue);
	}
	else
	{
		UE_LOG(LogTemp, Error,
		TEXT("APingPongPlayerController::Server_PlatformMoveRight_Implementation: HAS NO PLATFORM!"));
	}
}

bool APingPongPlayerController::Server_PlatformMoveRight_Validate(float AxisValue)
{
	return true;
}

void APingPongPlayerController::SpawnPlatform_Implementation(TSubclassOf<APingPongPlatform> PlatfromClass)
{
	Platform = (APingPongPlatform*)GetWorld()->SpawnActor<APingPongPlatform>(PlatfromClass);
    if(Platform)
    {
    	Platform->SetActorLocation(StartTransform.GetLocation());
    	Platform->SetActorRotation(StartTransform.GetRotation());
    }

}

bool APingPongPlayerController::SpawnPlatform_Validate(TSubclassOf<APingPongPlatform> PlatfromClass)
{
	return PlatformClass != NULL;
}
