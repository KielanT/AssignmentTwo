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

	Zone = CreateDefaultSubobject<UBoxComponent>(TEXT("Finish Zone"));
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
	Zone->OnComponentBeginOverlap.AddDynamic(this, &AFinishZoneActor::OnOverlapBegin);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), Characters);

	GameInstanceRef = Cast<UMultiplayerGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	CourseGameModeRef = Cast<ACourseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	ParticleOne->SetActive(bIsParticleActive);
	ParticleTwo->SetActive(bIsParticleActive);

}

// Called every frame
void AFinishZoneActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MulticastPlayParticles();
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
	if (PlayerTracker == 1)
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

	for (int i = 0; i < Characters.Num(); i++)
	{
		if (!CharactersCrossLine.Contains(OtherActor))
		{
			ABaseCharacter* Player = Cast<ABaseCharacter>(OtherActor);
			UE_LOG(LogTemp, Warning, TEXT("Crossed Line"));
			CharactersCrossLine.Add(Player);
			PlayerTracker++;
			CourseGameModeRef->SetNumberOfPlayersFinished(PlayerTracker);

			Player->ClientFinishSound();

			APlayerController* PC = GetWorld()->GetFirstPlayerController();
			ACustomPlayerController* CPC = Cast<ACustomPlayerController>(PC);

			//FString CountText = FString::FromInt(PlayerTracker);
			//CPC->HUD->SetFinishCountText(FText::FromString(CountText));
			ServerUpdateText();
		}
	}




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
