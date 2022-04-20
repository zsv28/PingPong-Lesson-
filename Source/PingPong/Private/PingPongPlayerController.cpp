// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongPlayerController.h"
#include "PingPongPlatform.h"
#include "Net/UnrealNetwork.h"

APingPongPlayerController::APingPongPlayerController()
{
	SetReplicates(true);
}
void APingPongPlayerController::SetStartTransfrorm(const FTransform &NewStartTransform)
{
	StartTransform = NewStartTransform;
}
void APingPongPlayerController::Player1Score_Implementation(int32 Score)
{
	Widget->Player1ScoreUpdate(Score);
}
void APingPongPlayerController::Player2Score_Implementation(int32 Score)
{
	Widget->Player2ScoreUpdate(Score);
}

void APingPongPlayerController::Client_HUDInit_Implementation()
{
	if (!Widget)
	{
		Widget = CreateWidget<UScoreWidget>(this, WidgetClass);
		if (Widget)
		{
			Widget->Player1ScoreUpdate(0);
			Widget->Player2ScoreUpdate(0);
			Widget->AddToViewport();
		}
	}
}

bool APingPongPlayerController::Client_HUDInit_Validate()
{
	return (WidgetClass != nullptr);
}

void APingPongPlayerController::Server_Init_Implementation(int32 NewPlayerID, APingPongGate* NewGate)
{
	if (Platform)
	{
		Platform->Destroy();
	}
	SpawnPlatform(PlatformClass);

	PlayerID = NewPlayerID;
	Gate = NewGate;
	Gate->SetPlayerID(PlayerID);
}

bool APingPongPlayerController::Server_Init_Validate(int32 NewPlayerID, APingPongGate* NewGate)
{
	return (NewPlayerID != 0 && NewGate != nullptr);
}

void APingPongPlayerController::Client_SetHUDPlayerIndex_Implementation(PlayerId playerIdex)
{
	if (Widget)
	{
		Widget->SwitchWidget(static_cast<int32>(playerIdex));
	}
}

void APingPongPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	InputComponent->BindAxis("Move", this, &APingPongPlayerController::MoveRight);
}
void APingPongPlayerController::MoveRight(float AxisValue)
{
	if(AxisValue != 0)
	{
	UE_LOG(LogTemp, Warning, TEXT("APingPongPlayerController::MoveRight"));
	}
	Server_PlatformMoveRight(AxisValue);
}

void APingPongPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(APingPongPlayerController, PlayerID, COND_SimulatedOnly);
}

bool APingPongPlayerController::Server_PlatformMoveRight_Validate(float AxisValue)
{
	return true;
}
void APingPongPlayerController::Server_PlatformMoveRight_Implementation(float AxisValue)
{
	if(Platform)
	{
		if(AxisValue != 0)
		{
			UE_LOG(LogTemp, Warning,TEXT("APingPongPlayerController::Server_PlatformMoveRight_Implementation"));
		}
	Platform->Server_MoveRight(AxisValue);
	}
	else
	{
		UE_LOG(LogTemp, Error,TEXT("APingPongPlayerController::Server_PlatformMoveRight_Implementation: HAS NO PLATFORM!"));
	}
}
bool APingPongPlayerController::SpawnPlatform_Validate(TSubclassOf<APingPongPlatform> PlatfromClass)
{
	return PlatfromClass != nullptr;
}
void APingPongPlayerController::SpawnPlatform_Implementation(TSubclassOf<APingPongPlatform> PlatfromClass)
{
	Platform = GetWorld()->SpawnActor<APingPongPlatform>(PlatfromClass);
	if(Platform)
	{
		Platform->SetActorLocation(StartTransform.GetLocation());
		Platform->SetActorRotation(StartTransform.GetRotation());
	}
}
