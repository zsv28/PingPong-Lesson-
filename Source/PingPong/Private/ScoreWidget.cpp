// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreWidget.h"

void UScoreWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UScoreWidget::Player1ScoreUpdate(int NewValue)
{
	if (T_Player1Score)
	{
		TArray<FStringFormatArg> Arg;
		Arg.Add(FStringFormatArg(NewValue));
		FString String { FString::Format(TEXT("Player_1: {0}") , Arg) };
		T_Player1Score->SetText(FText::FromString(String));
	}
}

void UScoreWidget::Player2ScoreUpdate(int NewValue)
{
	if (T_Player2Score)
	{
		TArray<FStringFormatArg> Arg;
		Arg.Add(FStringFormatArg(NewValue));
		FString String { FString::Format(TEXT("Player_2: {0}") , Arg) };
		T_Player2Score->SetText(FText::FromString(String));
	}
}
