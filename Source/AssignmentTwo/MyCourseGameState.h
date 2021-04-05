// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MyCourseGameState.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENTTWO_API AMyCourseGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AMyCourseGameState();

	UPROPERTY()
		uint32 NumberOfPlayersFinished = 0;

	UPROPERTY()
		uint32 NumberOfPlayers = 0;

	UFUNCTION()
		uint32 GetNumberOfPlayersFinished();

	UFUNCTION()
		uint32 GetNumberOfPlayers();
};
