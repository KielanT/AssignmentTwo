// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "CheckpointTriggerBox.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENTTWO_API ACheckpointTriggerBox : public ATriggerBox
{
	GENERATED_BODY()
	
public: 
	ACheckpointTriggerBox();

	UFUNCTION()
		void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor); // Used on overlap function

};
