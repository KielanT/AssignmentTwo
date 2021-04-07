// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SlidingDoorActor.generated.h"

UCLASS()
class ASSIGNMENTTWO_API ASlidingDoorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASlidingDoorActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

private:
	UPROPERTY(EditAnywhere, Replicated)
		UStaticMeshComponent* Mesh;

	UPROPERTY(Replicated)
		FVector CurrentPos;

	UPROPERTY(Replicated)
		FVector TargetPos;

	UPROPERTY(EditAnywhere, Replicated)
		USceneComponent* TopPos;

	UPROPERTY(EditAnywhere, Replicated)
		USceneComponent* BottomPos;

	UPROPERTY(EditAnywhere, Replicated)
		float Speed = 30;

	UPROPERTY(Replicated)
		FTimerHandle DoorWaitTimer;

	UPROPERTY(Replicated)
		float DoorWaitTime;

	UPROPERTY(Replicated)
		uint8 TimerTracker = 0;

private: // private Functions
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void ResetTarget();

	UFUNCTION(NetMulticast, Reliable, WithValidation) // RPC
		void MultiCastSlideDoor();


};
