// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongGate.h"

#include "PingPongGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


// Sets default values
APingPongGate::APingPongGate()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Gate BodyCollision"));
	SetRootComponent(BodyCollision);
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gate BodyMesh"));
	BodyMesh->SetupAttachment(RootComponent);
	BodyMesh->SetIsReplicated(true);
	SetReplicates(true);
}

// Called when the game starts or when spawned
void APingPongGate::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APingPongGate::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}
void APingPongGate::Server_GoalInGate_Implementation()
{
	APingPongGameModeBase* GameMode { Cast<APingPongGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())) };
	if (GameMode != nullptr)
	{
		GameMode->AddScore(PlayerID);
	}
}

bool APingPongGate::Server_GoalInGate_Validate()
{
	return true;
}
void APingPongGate::Server_SetPlayerID_Implementation(int32 ID)
 {
 	PlayerID = ID;
 }
 
 bool APingPongGate::Server_SetPlayerID_Validate(int32 ID)
 {
 	return ID != 0;
 }
void APingPongGate::GoalInGate()
{
	Server_GoalInGate();
}

void APingPongGate::SetPlayerID(int32 ID)
{
	Server_SetPlayerID(ID);
}

void APingPongGate::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(APingPongGate, PlayerID, COND_SimulatedOnly);
}




