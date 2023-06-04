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

protected:
	
    UPROPERTY()
    class APingPongPlayerController* Player1;
	
    UPROPERTY()
    class APingPongPlayerController* Player2;
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    APlayerStart* Player1Start;
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    APlayerStart* Player2Start;

	UPROPERTY()
	int32 ScorePlayer1 = 0;
	UPROPERTY()
	int32 ScorePlayer2 = 0;
	
public:
    APingPongGameModeBase();
	
    virtual void BeginPlay() override;
    virtual void PostLogin(APlayerController* NewPlayer) override;

	void PlayerGoal(int8 PlayerID);

private:
	bool StartGame();
};
