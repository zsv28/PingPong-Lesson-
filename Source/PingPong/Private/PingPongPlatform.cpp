// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongPlatform.h"

#include "Engine/AssetManager.h"


// Sets default values
APingPongPlatform::APingPongPlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BodyCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Platfrom Body Collider"));
	SetRootComponent(BodyCollision);
	
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform Body Mesh"));
	BodyMesh->SetupAttachment(RootComponent);
	BodyMesh->SetIsReplicated(true);
	SetReplicates(true);
	AActor::SetReplicateMovement(true);
}
// Called when the game starts or when spawned
void APingPongPlatform::BeginPlay()
{
	Super::BeginPlay();
}
// Called every frame
void APingPongPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APingPongPlatform::LoadBodyMesh()
{
	FStreamableDelegate LoadMeshDelegate;
	LoadMeshDelegate.BindUObject(this, &APingPongPlatform::BodyMeshLoaded);
	UAssetManager& AssetManager = UAssetManager::Get();
	FStreamableManager& StreamableManager = AssetManager.GetStreamableManager();
	AssetHandles = StreamableManager.RequestAsyncLoad(BodyMeshRef.ToSoftObjectPath(),LoadMeshDelegate);
}

void APingPongPlatform::BodyMeshLoaded() const
{
	if (UStaticMesh* LoadedMesh = Cast<UStaticMesh>(AssetHandles.Get()->GetLoadedAsset()))
	{
		BodyMesh->SetStaticMesh(LoadedMesh);
	}
}

bool APingPongPlatform::Server_MoveRight_Validate(float AxisValue)
{
	return true;
}
void APingPongPlatform::Server_MoveRight_Implementation(float AxisValue)
{
	if(AxisValue != 0)
	{
	UE_LOG(LogTemp, Warning, TEXT("APingPongPlatform::Server_MoveRight_Implementation"));
	}
	const FVector CurrLocation = GetActorLocation();
	const FVector NextLocation = GetActorLocation() + GetActorRightVector() * MoveSpeed * AxisValue;
	if(!SetActorLocation(NextLocation, true))
	{
		SetActorLocation(CurrLocation);
	}
}

