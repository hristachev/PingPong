// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "PingPongBall.generated.h"

struct FStreamableHandle;

UCLASS()
class PINGPONG_API APingPongBall : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USphereComponent* BodyCollision;
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* BodyMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftObjectPtr<UStaticMesh> BodyMeshRef;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
<<<<<<< Updated upstream
	UMaterial* BodyMaterial;
=======
	class UMaterial* BodyMaterial;
>>>>>>> Stashed changes

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftObjectPtr<UMaterial> BodyMaterialRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball params")
	float MoveSpeed = 100;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball params")
	UParticleSystem* HitEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftObjectPtr<UParticleSystem> HitEffectRef;
	
	UPROPERTY(Replicated)
	bool IsMoving = true;

	TArray<TSharedPtr<FStreamableHandle>> AssetHandles;
	
public:	
	// Sets default values for this actor's properties
	APingPongBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

<<<<<<< Updated upstream
	UFUNCTION(BlueprintCallable, CallInEditor)
	void LoadBodyMesh();

	void OnBodyMeshLoaded();

	UMaterial* LoadBodyMaterial();

	void LoadHitEffect();

	void OnHitEffectLoaded();
	
=======
	void LoadBodyResources(UStaticMesh*& OutBodyMesh, UMaterial*& OutBodyMaterial);

>>>>>>> Stashed changes
	UFUNCTION(Server, Reliable, WithValidation)
    void Server_Move(float DeltaTime);
    UFUNCTION(Server, Reliable, WithValidation)
    void Server_StartMove();
    UFUNCTION(Server, Reliable, WithValidation)
    void Server_StopMove();
    UFUNCTION(NetMulticast, Unreliable)
    void Multicast_HitEffect();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
    void StartMove();
	
    UFUNCTION(BlueprintCallable)
    void StopMove();
	
    virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > &OutLifetimeProps) const override;
};
