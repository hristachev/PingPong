// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "PingPongPlatform.generated.h"

struct FStreamableHandle;

UCLASS()
class PINGPONG_API APingPongPlatform : public AActor
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent * BodyCollision;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftObjectPtr<UStaticMesh> BodyMeshRef;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed = 100;

	TSharedPtr<FStreamableHandle> AssetHandle;

public:	
	// Sets default values for this actor's properties
	APingPongPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveRight(float AxisValue);

	void LoadBodyMesh();

	void OnBodyMeshLoaded();
};
