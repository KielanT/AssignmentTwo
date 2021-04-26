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
	UPROPERTY(EditAnywhere)
		USceneComponent* Root;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

	UPROPERTY(Replicated)
		bool bIsSlideDoor;

	UPROPERTY(EditAnywhere)
		float TopOffset;

	UPROPERTY(EditAnywhere)
		float BottomOffset;

	UPROPERTY()
		FVector CurrentPos;

	UPROPERTY()
		FVector TopPos;

	UPROPERTY()
		FVector BottomPos;

	UPROPERTY(Replicated)
		FVector TargetPos;

	UPROPERTY()
		int TimerTracker = 0;

	UPROPERTY(Replicated)
		float DoorWaitTime;

	UPROPERTY()
		FTimerHandle DoorWaitTimer;

	UFUNCTION()
		void SlideDoor();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void MulticastStartSlideDoor();
	
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void MulticastUpdateSlideDoor();
};
