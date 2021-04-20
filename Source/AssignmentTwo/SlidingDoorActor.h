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
		USceneComponent* Root;

	UPROPERTY(EditAnywhere, Replicated)
		UStaticMeshComponent* Mesh;

	UPROPERTY(Replicated)
		FVector TopPos;

	UPROPERTY(Replicated)
		FVector BottomPos;

	UPROPERTY(EditAnywhere, Replicated)
		float TopOffset;

	UPROPERTY(EditAnywhere, Replicated)
		float BottomOffset;

	UPROPERTY(Replicated)
		FVector CurrentPos;

	UPROPERTY(Replicated)
		FVector TargetPos;

	UPROPERTY(Replicated)
		int TimerTracker = 0;

	UPROPERTY(Replicated)
		float DoorWaitTime;

	UPROPERTY(Replicated)
		FTimerHandle DoorWaitTimer;

	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void MulticastSlideDoor();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void MulticastResetSlideDoor();
};
