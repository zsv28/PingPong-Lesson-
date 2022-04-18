// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongBall.h"

#include "DrawDebugHelpers.h"
#include "PingPongGate.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"




// Sets default values
APingPongBall::APingPongBall()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BodyCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Ball BodyCollider"));
	SetRootComponent(BodyCollision);
	
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball BodyMesh"));
	BodyMesh->SetupAttachment(RootComponent);
	BodyMesh->SetIsReplicated(true);
	
	
	SetReplicates(true);
	SetReplicateMovement(true);
}
// Called when the game starts or when spawned
void APingPongBall::BeginPlay()
{
	Super::BeginPlay();
	
}
// Called every frame
void APingPongBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetNetMode() != ENetMode::NM_Client)
	{
		Server_Move(DeltaTime);
	}
}
void APingPongBall::StartMove()
{
	Server_StartMove();
}
void APingPongBall::StopMove()
{
	Server_StopMove();
}
void APingPongBall::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APingPongBall, IsMoving);
}
bool APingPongBall::Server_Move_Validate(float DeltaTime)
{
	return true;
}
void APingPongBall::Server_Move_Implementation(float DeltaTime)
{
	const FVector Forward = GetActorForwardVector();
	const FVector CurrLoc = GetActorLocation();
	const float CoordinatZ = CurrLoc.Z;
	const FVector NewLoc = CurrLoc + Forward * MoveSpeed * DeltaTime;
	FHitResult HitResult;
	if(!SetActorLocation(NewLoc, true, &HitResult))
	{
		if (auto Gate = Cast<APingPongGate>(HitResult.GetActor()))
		{
			UE_LOG(LogTemp, Warning, TEXT("GOAL"));
			Gate->GoalInGate();
		}
		UE_LOG(LogTemp, Warning, TEXT("Ball %s Collided with %s"), *GetName(),*HitResult.GetActor()->GetName());
		FVector MoveVector = Forward - CurrLoc;
		MoveVector.Normalize();
		FVector ResetPosition = CurrLoc + MoveVector * DeltaTime * 5 * MoveSpeed;
		ResetPosition.Z = CoordinatZ;
		DrawDebugDirectionalArrow(GetWorld(), NewLoc + MoveVector * 300, NewLoc,30, FColor::Yellow, true, 3.f, 0, 3);
		FVector ImpactCorrection = HitResult.ImpactPoint + HitResult.ImpactNormal * 300;
		DrawDebugDirectionalArrow(GetWorld(), HitResult.ImpactPoint,HitResult.ImpactPoint + HitResult.ImpactNormal * 300, 30, FColor::Blue, true, 3, 0,3);
		const float AimAtAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(MoveVector,HitResult.ImpactNormal)));
        MoveVector = MoveVector.RotateAngleAxis(AimAtAngle*2, FVector(0,0,1));
        FVector NewTargetMove = NewLoc + MoveVector * 300;
        NewTargetMove.Z = CurrLoc.Z;
        DrawDebugDirectionalArrow(GetWorld(), NewLoc, NewTargetMove, 30,FColor::Yellow, true, 3.f, 0, 3);
        //SetActorLocation(CurrLoc);
        SetActorLocation(ResetPosition);
		const FRotator CurrRotation = GetActorRotation();
        FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(CurrLoc,NewTargetMove);
        NewRotation.Pitch = CurrRotation.Pitch;
        //NewRotation.Yaw = NewRotation.Yaw + FMath::RandRange(-10, 10);
        SetActorRotation(NewRotation);
        Multicast_HitEffect();
	}
}
bool APingPongBall::Server_StartMove_Validate()
{
	return true;
}
void APingPongBall::Server_StartMove_Implementation()
{
	IsMoving = true;
}
bool APingPongBall::Server_StopMove_Validate()
{
	return true;
}
void APingPongBall::Server_StopMove_Implementation()
{
	IsMoving = false;
}
void APingPongBall::Multicast_HitEffect_Implementation()
{
	const UWorld * World = GetWorld();
	if(World && HitEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect,GetActorLocation());
	}
}
void APingPongBall::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector NewLocation(0.0f,0.0f,70.0f);
	if (OtherActor->IsA(APingPongGate::StaticClass()))
	{
 		SetActorLocation(NewLocation);
 	}
}

