// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PingPongGoal.generated.h"

class UBoxComponent;
UCLASS()
class PINGPONG_API APingPongGoal : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* BoxCollision;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(Replicated)
	int8 PlayerID = 0;
public:	
	// Sets default values for this actor's properties
	APingPongGoal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_GoalCheck();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetPlayerID(int8 ID);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void GoalCheck();

	void SetPlayerID(int8 ID);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class APlayerStart* PlayerStart;
};
