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

	APingPongGameModeBase* CurrentGM = Cast<APingPongGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	
	if (Cast<APingPongPlayerController>(GetOwningPlayer()) == CurrentGM->GetPlayer1())
	{
		if (PlayerScore && OpponentScore)
		{
			PlayerScore->SetText(FText::FromString(FString::FromInt(CurrentGM->GetScorePlayer1())));
			OpponentScore->SetText(FText::FromString(FString::FromInt(CurrentGM->GetScorePlayer2())));
		}
	}
	if (Cast<APingPongPlayerController>(GetOwningPlayer()) == CurrentGM->GetPlayer2())
	{
		if (PlayerScore && OpponentScore)
		{
			PlayerScore->SetText(FText::FromString(FString::FromInt(CurrentGM->GetScorePlayer2())));
			OpponentScore->SetText(FText::FromString(FString::FromInt(CurrentGM->GetScorePlayer1())));
		}
	}
}
