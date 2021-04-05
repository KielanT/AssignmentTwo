// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class ASSIGNMENTTWO_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private: // Functions
	void MoveForwards(float axis); // Function for forwards and backwards controls
	void Strafe(float axis); // Function for left and right controls
	void LookUp(float axis); // Function for looking up and down
	void Turn(float axis); // Function for turning left and right
	void Dive();
	void Push();

	UFUNCTION(Server, Reliable, WithValidation) // RPC
		void ServerDive();

	UFUNCTION(Server, Reliable, WithValidation) // RPC
		void ServerPush();

private: // Variables
	UPROPERTY(EditAnywhere)
		UCameraComponent* Camera; // Creates a camera component

	UPROPERTY(EditAnywhere)
		USpringArmComponent* SpringArm;

	const float MAX_DIVE_STRENGTH = 1000;

	UPROPERTY(EditAnywhere)
		float DiveStrength = 1000;

	const float MAX_PUSH_LENGTH = 100;
	const float MAX_PUSH_STRENGTH = 1000;

	UPROPERTY(EditAnywhere)
		float PushLength = 1000;

	UPROPERTY(EditAnywhere)
		float PushStrength = 1000;

	UPROPERTY(EditAnywhere)
		USceneComponent* PushStartComp;

public:
	UPROPERTY()
		FVector CheckpointLocation;
};