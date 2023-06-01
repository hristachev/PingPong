// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreWidget.h"

#include <ThirdParty/CryptoPP/5.6.5/include/misc.h>

#include "PingPongGameModeBase.h"
#include "PingPongPlayerController.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UScoreWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UScoreWidget::UpdatePlayerScore(int newValue)
{
	if (PlayerScore)
	{
		TArray<FStringFormatArg> Args;
		Args.Add(FStringFormatArg(newValue));
		auto String { FString::Format(TEXT("{0}") , Args) };
		PlayerScore->SetText(FText::FromString(String));
	}
}

void UScoreWidget::UpdateEnemyScore(int newValue)
{
	if (OpponentScore)
	{
		TArray<FStringFormatArg> Args;
		Args.Add(FStringFormatArg(newValue));
		auto String { FString::Format(TEXT("{0}") , Args) };
		OpponentScore->SetText(FText::FromString(String));
	}
}
