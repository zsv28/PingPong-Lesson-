// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PingPongGate.h"
#include "ScoreWidget.h"
#include "GameFramework/PlayerController.h"
#include "PingPongPlayerController.generated.h"

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
public:
	APingPongPlayerController();
	
	UFUNCTION()
	void SetStartTransfrorm(const FTransform &NewStartTransform);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void SpawnPlatform(TSubclassOf<APingPongPlatform> PlatfromClass);

	UFUNCTION(Client, Reliable)
	void Player1Score(int32 Score);

	UFUNCTION(Client, Reliable)
	void Player2Score(int32 Score);

	UFUNCTION(Client, Reliable, WithValidation)
	void Client_HUDInit();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Init(int32 NewPlayerID, APingPongGate* NewGate);
	
	UFUNCTION()
	FORCEINLINE int32 GetPlayerID() const { return PlayerID; }
	
	virtual void SetupInputComponent() override;

	
protected:
	UFUNCTION()
	void MoveRight(float AxisValue);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_PlatformMoveRight(float AxisValue);
	
	UPROPERTY(Replicated)
	int32 PlayerID = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	APingPongGate* Gate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UUserWidget> WidgetClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UScoreWidget* Widget;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
