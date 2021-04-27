// Fill out your copyright notice in the Description page of Project Settings.


#include "CourseGameMode.h"
#include "MyCourseGameState.h"
#include "MultiplayerGameInstance.h"
#include "Kismet/GameplayStatics.h"

ACourseGameMode::ACourseGameMode()
{
	GameStateClass = AMyCourseGameState::StaticClass();

}

void ACourseGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++NumberOfPlayers; // Increments when a player joins
	SetNumberOfPlayers(NumberOfPlayers); // Sets the number of players
}

void ACourseGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	--NumberOfPlayers; // Decrements when a player leaves
	SetNumberOfPlayers(NumberOfPlayers); // Sets the number of players
}

uint32 ACourseGameMode::GetNumberOfPlayers() const
{
	return GetGameState<AMyCourseGameState>()->NumberOfPlayers; // Gets the number of players
}

uint32 ACourseGameMode::GetNumberOfPlayersFinished() const
{
	return NumberOfPlayersFinished; // Gets ht enumber of players finished
}

void ACourseGameMode::SetNumberOfPlayersFinished(uint32 numPlayers)
{
	NumberOfPlayersFinished = numPlayers; // Sets the number of players that have finished
}

void ACourseGameMode::SetNumberOfPlayers(uint32 numPlayers)
{
	GetGameState<AMyCourseGameState>()->NumberOfPlayers = numPlayers; // Sets the number of players
}

void ACourseGameMode::BeginPlay()
{
	//UMultiplayerGameInstance* GameInstanceRef = Cast<UMultiplayerGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//GameInstanceRef->LoadHUDWidget();
}
