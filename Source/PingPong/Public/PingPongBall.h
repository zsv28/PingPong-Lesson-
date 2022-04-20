// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "PingPongBall.generated.h"

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
	class UMaterial* BodyMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftObjectPtr<UMaterial> BodyMaterialRef;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball params")
	float MoveSpeed = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball params")
	UParticleSystem* HitEffect;

	UPROPERTY(Replicated)
	bool IsMoving = false;
	
public:
	APingPongBall();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Move(float DeltaTime);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StartMove();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StopMove();

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_HitEffect();

	void LoadBodyRes(UStaticMesh*& Mesh, UMaterial*& Material);
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void StartMove();

	UFUNCTION(BlueprintCallable)
	void StopMove();

	virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > &OutLifetimeProps) const override;

protected:
	
	UStaticMesh* LoadMesh = nullptr;
	UMaterial* LoadMaterial = nullptr;
};
