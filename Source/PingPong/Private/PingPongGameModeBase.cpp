// Copyright Epic Games, Inc. All Rights Reserved.


#include "PingPongGameModeBase.h"
#include "PingPongGate.h"
#include "PingPongPlayerController.h"
#include "PingPongPlayerPawn.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

namespace utils
{
	template<typename T>
	void FindAllActors(UWorld* World, TArray<T*>& Out)
	{
		for (TActorIterator<T> It(World); It; ++It)
		{
			Out.Add(*It);
		}
	}
}

APingPongGameModeBase::APingPongGameModeBase()
{
	DefaultPawnClass = APingPongPlayerPawn::StaticClass();
	PlayerControllerClass = APingPongPlayerController::StaticClass();
}
void APingPongGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}
void APingPongGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	UWorld* World = GetWorld();
	
	if(World && (!Player1Start || !Player2Start))
	{
		TArray<APlayerStart*> FoundActors;
		utils::FindAllActors<APlayerStart>(World, FoundActors);
		if(FoundActors.Num() > 0)	
			Player1Start = Cast<APlayerStart>(FoundActors[0]);
		if(FoundActors.Num() > 1)
			Player2Start = Cast<APlayerStart>(FoundActors[1]);
	}
	
	APingPongPlayerController* CurrPlayer;
	const APlayerStart* StartPos;
	int32 PlayerID;
	
	if(Player1 == nullptr)
	{
		Player1 = Cast<APingPongPlayerController>(NewPlayer);
		PlayerID = 1;
		CurrPlayer = Player1;
		StartPos = Player1Start;
		UE_LOG(LogTemp, Warning, TEXT("PingPongGameMode: Init player 1"));
	}
	else if(Player2 == nullptr)
	{
		Player2 = Cast<APingPongPlayerController>(NewPlayer);
		PlayerID = 2;
		CurrPlayer = Player2;
		StartPos = Player2Start;
		UE_LOG(LogTemp, Warning, TEXT("PingPongGameMode: Init player 2"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PingPongGameMode: There are already two players in the game. New connections are not possible"));
		return;
	}
	
	APingPongPlayerPawn* NewPawn = Cast<APingPongPlayerPawn>(NewPlayer->GetPawn());
	
	if(!NewPawn)
	{
		NewPawn = World->SpawnActor<APingPongPlayerPawn>(DefaultPawnClass);
	}

	TArray<APingPongGate*> FoundGateActors;
	utils::FindAllActors<APingPongGate>(World,FoundGateActors);
	auto FindGateByPlayerStart = [](const TArray<APingPongGate*>& Array, const APlayerStart* SearchObject)
	{
		auto Result = Array.FindByPredicate([&](const APingPongGate *Gate)
		{
			return Gate->PlayerStart == SearchObject;
		});
		return Result != nullptr ? *Result : nullptr;
	};
	if(StartPos && NewPawn)
	{
		APingPongGate* PlayerGate { FindGateByPlayerStart(FoundGateActors, StartPos) }; 
		NewPawn->SetActorLocation(StartPos->GetActorLocation());
		NewPawn->SetActorRotation(StartPos->GetActorRotation());
		NewPlayer->SetPawn(NewPawn);
		CurrPlayer->SetStartTransfrorm(StartPos->GetActorTransform());
		CurrPlayer->Client_HUDInit();
		CurrPlayer->Server_Init(PlayerID, PlayerGate);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Start position not setted in PingPongGameMode!"));
	}
}

void APingPongGameModeBase::AddScore(int32 PlayerID)
{
	
	if (PlayerID == 1)
	{
		CurrentScore = ++PlayerScore1;
	}
	else if (PlayerID == 2)
	{
		CurrentScore = ++PlayerScore2;
	}
	
	for (auto Iter {GetWorld()->GetPlayerControllerIterator()}; Iter; ++Iter)
	{
		APingPongPlayerController* PlayerController { Cast<APingPongPlayerController>(*Iter) };
		if (PlayerController != nullptr)
		{
			if (PlayerController->GetPlayerID() == PlayerID)
			{
				PlayerController->Player1Score(CurrentScore);
			}
			else
			{
				PlayerController->Player2Score(CurrentScore);
			}
		}
	}
	
}



