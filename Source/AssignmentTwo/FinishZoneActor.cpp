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

	Zone = CreateDefaultSubobject<UBoxComponent>(TEXT("Finish Zone")); // The collision zone
	Zone->SetupAttachment(RootComponent);

	ParticleOne = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Particle One")); 
	ParticleOne->SetupAttachment(Zone);

	ParticleTwo = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Particle Two"));
	ParticleTwo->SetupAttachment(Zone);
	bReplicates = true;
}

void AFinishZoneActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicates the variables
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
	Zone->OnComponentBeginOverlap.AddDynamic(this, &AFinishZoneActor::OnOverlapBegin); // Binds the on overlap fucntion
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), Characters);

	GameInstanceRef = Cast<UMultiplayerGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())); // Gets game instance
	CourseGameModeRef = Cast<ACourseGameMode>(UGameplayStatics::GetGameMode(GetWorld())); // Gets game mode ref

	ParticleOne->SetActive(bIsParticleActive); // Enables/ Disables the particles  (should be off by default)
	ParticleTwo->SetActive(bIsParticleActive); // Enables/ Disables the particles  (should be off by default)

}

// Called every frame
void AFinishZoneActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MulticastPlayParticles(); // Calls the play particle function every tick
}

void AFinishZoneActor::ServerUpdateText_Implementation() // Updatee the text
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
	if (PlayerTracker == 1) // Activates the particles if one player has finished
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

	for (int i = 0; i < Characters.Num(); i++) // Loops for each character in the game
	{
		if (!CharactersCrossLine.Contains(OtherActor)) // Checks if the player has not crossed the line
		{
			ABaseCharacter* Player = Cast<ABaseCharacter>(OtherActor);
			UE_LOG(LogTemp, Warning, TEXT("Crossed Line"));
			CharactersCrossLine.Add(Player); // Adds the player to the array
			PlayerTracker++; // Increments the player tracker
			CourseGameModeRef->SetNumberOfPlayersFinished(PlayerTracker); // Sets the number of players finished

			Player->ClientFinishSound(); // Plays finish sound on client only

			//APlayerController* PC = GetWorld()->GetFirstPlayerController();
			//ACustomPlayerController* CPC = Cast<ACustomPlayerController>(PC);

			//FString CountText = FString::FromInt(PlayerTracker);
			//CPC->HUD->SetFinishCountText(FText::FromString(CountText));
			//ServerUpdateText();
		}
	}

	if (PlayerTracker >= 3) // If all players have finished then the server changes level
	{
		UE_LOG(LogTemp, Warning, TEXT("Players finished"));
		ServerChangeLevel();
	}

}

void AFinishZoneActor::ServerChangeLevel_Implementation()
{
	GameInstanceRef->Warp(TEXT("EndLevel")); // Changes the level to the end level
}

bool AFinishZoneActor::ServerChangeLevel_Validate()
{
	return true;
}
