// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreWidget.generated.h"

class UTextBlock;
class UWidgetSwitcher;
/**
 * 
 */
UCLASS()
class PINGPONG_API UScoreWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* PlayerScore;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* OpponentScore;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* WidgetSwitcher;

public:

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void UpdatePlayerScore(int newValue);

	UFUNCTION(BlueprintCallable)
	void UpdateEnemyScore(int newValue);
	
	UFUNCTION(BlueprintCallable)
	void SwitchWidget(int32 id);
};
