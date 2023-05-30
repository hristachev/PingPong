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
	
public:	
	// Sets default values for this actor's properties
	APingPongGoal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(Server, Reliable)
	void Server_CheckOverlap(AActor* _ActorRef, bool _bOverlapEnd);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
