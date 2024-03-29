// Fill out your copyright notice in the Description page of Project Settings.


#include "SpinningBarActor.h"
#include "NET/UnrealNetwork.h"

// Sets default values
ASpinningBarActor::ASpinningBarActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	bReplicates = true;
}

// Called when the game starts or when spawned
void ASpinningBarActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		NetUpdateFrequency = 5;
		SetReplicates(true);
		SetReplicateMovement(true);
	}
}

// Called every frame
void ASpinningBarActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorLocalRotation(FRotator(0.0f, RotationRate * DeltaTime, 0.0f)); // Rotates the spin bar
}

void ASpinningBarActor::SpinBar()
{
	ServerSpinBar();
}

void ASpinningBarActor::ServerSpinBar_Implementation()
{
	AddActorLocalRotation(FRotator(0.0f, RotationRate * GetWorld()->DeltaTimeSeconds, 0.0f));
}

bool ASpinningBarActor::ServerSpinBar_Validate()
{
	if (RotationRate <= MAX_ROTATION_RATE)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ASpinningBarActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicates the rotation rate
	DOREPLIFETIME(ASpinningBarActor, RotationRate);
}

