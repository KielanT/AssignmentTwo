// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpinningBarActor.generated.h"

UCLASS()
class ASSIGNMENTTWO_API ASpinningBarActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpinningBarActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private: // Private Functions
	UFUNCTION()
		void SpinBar();

	UFUNCTION(Server, Reliable, WithValidation) // RPC
		void ServerSpinBar();

	
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

private: // Variables
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Replicated)
		float RotationRate = 200.0f;

	const float MAX_ROTATION_RATE = 200.0f;
};
