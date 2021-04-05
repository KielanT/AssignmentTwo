// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "SafetyNetTriggerBox.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENTTWO_API ASafetyNetTriggerBox : public ATriggerBox
{
	GENERATED_BODY()

public:
	ASafetyNetTriggerBox();

	UFUNCTION()
		void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor); // Used on overlap function
};
