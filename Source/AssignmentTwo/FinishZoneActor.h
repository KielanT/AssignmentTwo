// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FinishZoneActor.generated.h"

class UBoxComponent;
class ABaseCharacter;
class UMultiplayerGameInstance;
class ACourseGameMode;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class ASSIGNMENTTWO_API AFinishZoneActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFinishZoneActor();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const; // Used for replicating variables

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Replicated)
		UBoxComponent* Zone; // Collision Zone

	UPROPERTY(Replicated)
		TArray<AActor*> Characters; // Array of the characters

	UPROPERTY(Replicated)
		TArray<ABaseCharacter*> CharactersCrossLine; // Array of the characters crossed the line

	UPROPERTY(Replicated)
		int PlayerTracker = 0; // Player tracker

	UPROPERTY()
		UMultiplayerGameInstance* GameInstanceRef; 



	UPROPERTY()
		ACourseGameMode* CourseGameModeRef;

	UPROPERTY(EditAnywhere, Replicated)
		UNiagaraComponent* ParticleOne; // Niagara particles

	UPROPERTY(EditAnywhere, Replicated)
		UNiagaraComponent* ParticleTwo;

	UPROPERTY(Replicated)
		bool bIsParticleActive = false; // Used for enabling and disabling particles


	UFUNCTION(Server, Reliable)
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Server, Reliable, WithValidation) // RPC
		void ServerChangeLevel();

	UFUNCTION(NetMulticast, Reliable) // RPC
		void MulticastPlayParticles();

	UFUNCTION(Server, Reliable, WithValidation) // RPC
		void ServerUpdateText();
};
