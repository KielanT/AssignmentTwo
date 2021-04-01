// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MultiplayerGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENTTWO_API UMultiplayerGameInstance : public UGameInstance
{
	GENERATED_BODY()


public:
	UFUNCTION(Exec)
		void Host();

	UFUNCTION(Exec)
		void Join(const FString IPAddress);

	UFUNCTION(Exec)
		void Warp(const FString LevelName);
};
