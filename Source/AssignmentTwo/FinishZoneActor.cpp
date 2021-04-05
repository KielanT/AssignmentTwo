// Fill out your copyright notice in the Description page of Project Settings.


#include "FinishZoneActor.h"
#include "Components/BoxComponent.h"
#include "BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "MultiplayerGameInstance.h"
#include "CourseGameMode.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AFinishZoneActor::AFinishZoneActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Zone = CreateDefaultSubobject<UBoxComponent>(TEXT("Finish Zone"));
	Zone->SetupAttachment(RootComponent);

	ParticleOneSpawner = CreateDefaultSubobject<USceneComponent>(TEXT("Particle One Spawn"));
	ParticleOneSpawner->SetupAttachment(RootComponent);

	ParticleTwoSpawner = CreateDefaultSubobject<USceneComponent>(TEXT("Particle Two Spawn"));
	ParticleTwoSpawner->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFinishZoneActor::BeginPlay()
{
	Super::BeginPlay();
	Zone->OnComponentBeginOverlap.AddDynamic(this, &AFinishZoneActor::OnOverlapBegin);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), Characters);

	GameInstanceRef = Cast<UMultiplayerGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	CourseGameModeRef = Cast<ACourseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

// Called every frame
void AFinishZoneActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFinishZoneActor::MulticastPlayParticles_Implementation()
{
	if (PlayerTracker == 1)
	{
		FVector ParticleOneLocation = GetActorLocation();
		ParticleOneLocation.X += ParticleOffsetX;
		ParticleOneLocation.Y += ParticleOffsetY;
		ParticleOneLocation.Z += ParticleOffsetZ;

		FVector ParticleTwoLocation = GetActorLocation();
		ParticleTwoLocation.X -= ParticleOffsetX;
		ParticleTwoLocation.Y -= ParticleOffsetY;
		ParticleTwoLocation.Z += ParticleOffsetZ;

		if (ParticleSystemOne != nullptr && ParticleSystemTwo != nullptr)
		{
			ParticleOne = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ParticleSystemOne, ParticleOneLocation, GetActorRotation());
			ParticleTwo = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ParticleSystemTwo, ParticleTwoLocation, GetActorRotation());
		}
	}
}

void AFinishZoneActor::ClientFinishSound_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FinishSound, GetActorLocation());
}



void AFinishZoneActor::OnOverlapBegin_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Finished"));

	for (int i = 0; i < Characters.Num(); i++)
	{
		if (!CharactersCrossLine.Contains(OtherActor))
		{
			CharactersCrossLine.Add(Cast<ABaseCharacter>(OtherActor));
			PlayerTracker++;
			CourseGameModeRef->SetNumberOfPlayersFinished(PlayerTracker);
			ClientFinishSound();

			
		}
	}

	MulticastPlayParticles();
	
	
	if (PlayerTracker >= 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("Players finished"));
		ServerChangeLevel();
	}
}


void AFinishZoneActor::ServerChangeLevel_Implementation()
{
	GameInstanceRef->Warp(TEXT("EndLevel"));
}

bool AFinishZoneActor::ServerChangeLevel_Validate()
{
	return true;
}
