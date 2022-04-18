// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "ScoreWidget.generated.h"

/**
 * 
 */
UCLASS()
class PINGPONG_API UScoreWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* T_Player1Score;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* T_Player2Score;
	
public:
	
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void Player1ScoreUpdate(int NewValue);
	
	UFUNCTION(BlueprintCallable)
	void Player2ScoreUpdate(int NewValue);
};

