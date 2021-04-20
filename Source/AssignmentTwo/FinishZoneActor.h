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

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Replicated)
		UBoxComponent* Zone;

	UPROPERTY(Replicated)
		TArray<AActor*> Characters;

	UPROPERTY(Replicated)
		TArray<ABaseCharacter*> CharactersCrossLine;

	UPROPERTY(Replicated)
		int PlayerTracker = 0;

	UPROPERTY()
		UMultiplayerGameInstance* GameInstanceRef;



	UPROPERTY()
		ACourseGameMode* CourseGameModeRef;

	UPROPERTY(EditAnywhere, Replicated)
		UNiagaraComponent* ParticleOne;

	UPROPERTY(EditAnywhere, Replicated)
		UNiagaraComponent* ParticleTwo;

	UPROPERTY(Replicated)
		bool bIsParticleActive = false;


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
