// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PingPongGate.h"
#include "GameFramework/GameModeBase.h"
#include "PingPongGameModeBase.generated.h"


/**
 * 
 */
UCLASS()
class PINGPONG_API APingPongGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	class APingPongPlayerController* Player1 = nullptr;
	
    UPROPERTY()
    class APingPongPlayerController* Player2 = nullptr;
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    APlayerStart* Player1Start;
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    APlayerStart* Player2Start;

	UPROPERTY()
	int32 PlayerScore1 = 0;
	UPROPERTY()
	int32 PlayerScore2 = 0;

	int32 CurrentScore = 0;

public:
    APingPongGameModeBase();
    virtual void BeginPlay() override;
    virtual void PostLogin(APlayerController* NewPlayer) override;
	
	
	UFUNCTION(BlueprintCallable)
	void AddScore(int32 PlayerID);

	
};
