// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerStart.h"

#include "PingPongGameModeBase.generated.h"

class UScoreWidget;
/**
 * 
 */
UCLASS()
class PINGPONG_API APingPongGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

private:
	int ScorePlayer1 = 0;
	int ScorePlayer2 = 0;
	
	
protected:
	
    UPROPERTY()
    class APingPongPlayerController* Player1 = NULL;
	
    UPROPERTY()
    class APingPongPlayerController* Player2 = NULL;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> ScoreWidgetClass;

	UPROPERTY()
	UScoreWidget* ScoreWidget;
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    APlayerStart* Player1Start;
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    APlayerStart* Player2Start;
	
public:
    APingPongGameModeBase();
	
    virtual void BeginPlay() override;
    virtual void PostLogin(APlayerController* NewPlayer) override;

	int GetScorePlayer1() { return ScorePlayer1; }
	int GetScorePlayer2() { return ScorePlayer2; }
	APingPongPlayerController* GetPlayer1() { return Player1; }
	APingPongPlayerController* GetPlayer2() { return Player2; }
	
	UFUNCTION()
	void AddScore(int ScoreIncrease);
};
