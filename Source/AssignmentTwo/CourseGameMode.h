// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CourseGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENTTWO_API ACourseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACourseGameMode();

	void PostLogin(APlayerController* NewPlayer) override;
	void Logout(AController* Exiting) override;

	UFUNCTION()
		uint32 GetNumberOfPlayers() const;

	UFUNCTION()
		uint32 GetNumberOfPlayersFinished() const;

	UFUNCTION()
		void SetNumberOfPlayersFinished(uint32 numPlayers);

	UFUNCTION()
		void SetNumberOfPlayers(uint32 numPlayers);

	

private:
	uint32 NumberOfPlayersFinished = 0;

	uint32 NumberOfPlayers = 0;

	
};
