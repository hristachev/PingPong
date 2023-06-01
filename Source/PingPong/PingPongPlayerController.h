// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PingPongPlayerController.generated.h"

class UScoreWidget;
class APingPongGoal;
/**
 * 
 */
UCLASS()
class PINGPONG_API APingPongPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	
	UPROPERTY()
	FTransform StartTransform;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class APingPongPlatform> PlatformClass;
	
	UPROPERTY()
	class APingPongPlatform* Platform;

	UPROPERTY(Replicated)
	int8 PlayerID = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	APingPongGoal* PongGoal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UScoreWidget* ScoreWidget;
	
public:
	
	APingPongPlayerController();
	
	UFUNCTION()
	void SetStartTransfrorm(FTransform NewStartTransform);

	UFUNCTION()
	FORCEINLINE int8 GetPlayerID() const { return PlayerID; }
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Initialize(int8 NewPlayerID, APingPongGoal* NewGoal);

	UFUNCTION(Client, Reliable, WithValidation)
	void Client_InitializeHUD();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void SpawnPlatform(TSubclassOf<class APingPongPlatform> PlatfromClass);

	UFUNCTION(Client, Reliable)
	void UpdateWidgetPlayerScore(int32 Score);

	UFUNCTION(Client, Reliable)
	void UpdateWidgetEnemyScore(int32 Score);
	
	virtual void SetupInputComponent() override;
protected:
	
	UFUNCTION()
	void MoveRight(float AxisValue);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_PlatformMoveRight(float AxisValue);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
