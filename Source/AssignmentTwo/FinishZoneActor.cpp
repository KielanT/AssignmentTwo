// Fill out your copyright notice in the Description page of Project Settings.


#include "FinishZoneActor.h"
#include "Components/BoxComponent.h"
#include "BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "MultiplayerGameInstance.h"
#include "CourseGameMode.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "HUDWidget.h"
#include "NET/UnrealNetwork.h"
#include "CustomPlayerController.h"

// Sets default values
AFinishZoneActor::AFinishZoneActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Zone = CreateDefaultSubobject<UBoxComponent>(TEXT("Finish Zone")); // Creates the finsih zone
	Zone->SetupAttachment(RootComponent);

	ParticleOne = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Particle One")); // Create the particles
	ParticleOne->SetupAttachment(Zone);

	ParticleTwo = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Particle Two"));
	ParticleTwo->SetupAttachment(Zone);
	bReplicates = true; // Sets the actor to be replicated
}

void AFinishZoneActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Variable Replication
	DOREPLIFETIME(AFinishZoneActor, PlayerTracker);
	DOREPLIFETIME(AFinishZoneActor, ParticleOne);
	DOREPLIFETIME(AFinishZoneActor, ParticleTwo);
	DOREPLIFETIME(AFinishZoneActor, CharactersCrossLine);
	DOREPLIFETIME(AFinishZoneActor, Characters);
	DOREPLIFETIME(AFinishZoneActor, Zone);
	DOREPLIFETIME(AFinishZoneActor, bIsParticleActive);
}

// Called when the game starts or when spawned
void AFinishZoneActor::BeginPlay()
{
	Super::BeginPlay();
	Zone->OnComponentBeginOverlap.AddDynamic(this, &AFinishZoneActor::OnOverlapBegin); // Binds the collision function
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), Characters); // Gets all the character actors in the game and adds them to an array

	GameInstanceRef = Cast<UMultiplayerGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	CourseGameModeRef = Cast<ACourseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	ParticleOne->SetActive(bIsParticleActive); // Enables and Disables the particles
	ParticleTwo->SetActive(bIsParticleActive); // Enables and Disables the particles

}

// Called every frame
void AFinishZoneActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MulticastPlayParticles(); // Plays the particles on all clients and the server
}

void AFinishZoneActor::ServerUpdateText_Implementation()
{
	FString CountText = FString::FromInt(PlayerTracker);
	GameInstanceRef->HUD->SetFinishCountText(FText::FromString(CountText));
}

bool AFinishZoneActor::ServerUpdateText_Validate()
{
	return true;
}

void AFinishZoneActor::MulticastPlayParticles_Implementation()
{
	if (PlayerTracker == 1) // If the first player crosses the finish line activate the particles
	{
		bIsParticleActive = true;
		ParticleOne->SetActive(bIsParticleActive);
		ParticleTwo->SetActive(bIsParticleActive);
	}
}


void AFinishZoneActor::OnOverlapBegin_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Finished"));

	for (int i = 0; i < Characters.Num(); i++) // Loops for each player in the game
	{
		if (!CharactersCrossLine.Contains(OtherActor)) // Checks if the player has not already crossed the line
		{
			ABaseCharacter* Player = Cast<ABaseCharacter>(OtherActor);
			UE_LOG(LogTemp, Warning, TEXT("Crossed Line"));
			CharactersCrossLine.Add(Player); // Adds the player to an array
			PlayerTracker++; // Increments the player tracker 
			CourseGameModeRef->SetNumberOfPlayersFinished(PlayerTracker); // Sets the number of players finished

			Player->ClientFinishSound(); // Plays the finish sound on client only

			//APlayerController* PC = GetWorld()->GetFirstPlayerController();
			//ACustomPlayerController* CPC = Cast<ACustomPlayerController>(PC);

			//FString CountText = FString::FromInt(PlayerTracker);
			//CPC->HUD->SetFinishCountText(FText::FromString(CountText));
			//ServerUpdateText();
		}
	}




	if (PlayerTracker >= 3) // If 3 or more players have finished the line then change server
	{
		UE_LOG(LogTemp, Warning, TEXT("Players finished"));
		ServerChangeLevel();
	}

}

void AFinishZoneActor::ServerChangeLevel_Implementation()
{
	GameInstanceRef->Warp(TEXT("EndLevel")); // Moves to the final level
}

bool AFinishZoneActor::ServerChangeLevel_Validate()
{
	return true;
}
