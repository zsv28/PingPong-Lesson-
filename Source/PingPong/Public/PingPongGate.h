// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "PingPongGate.generated.h"


UCLASS()
class PINGPONG_API APingPongGate : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* BodyCollision;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* BodyMesh;
	
	UPROPERTY(Replicated)
	int32 PlayerID= 0; 
	
public:
	// Sets default values for this actor's properties
	APingPongGate();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class APlayerStart* PlayerStart;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_GoalInGate();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetPlayerID(int32 ID);
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void GoalInGate();

	void SetPlayerID(int32 ID);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};

