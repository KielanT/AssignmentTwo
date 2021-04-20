// Fill out your copyright notice in the Description page of Project Settings.


#include "CourseGameMode.h"
#include "MyCourseGameState.h"
#include "MultiplayerGameInstance.h"
#include "Kismet/GameplayStatics.h"

ACourseGameMode::ACourseGameMode()
{
	GameStateClass = AMyCourseGameState::StaticClass(); // Sets the game state class for this game mode
}

void ACourseGameMode::PostLogin(APlayerController* NewPlayer) // Runs everytime a player joins
{
	Super::PostLogin(NewPlayer);
	++NumberOfPlayers; // Increments everytime a player joins
	SetNumberOfPlayers(NumberOfPlayers); // Sets the number of players
}

void ACourseGameMode::Logout(AController* Exiting) // Runs everytime a player leaves
{
	Super::Logout(Exiting);
	--NumberOfPlayers; // Decrements each time a player leaves
	SetNumberOfPlayers(NumberOfPlayers); // Sets the number of players
}

uint32 ACourseGameMode::GetNumberOfPlayers() const
{
	return GetGameState<AMyCourseGameState>()->NumberOfPlayers; // Gets the number of players
}

uint32 ACourseGameMode::GetNumberOfPlayersFinished() const
{
	return NumberOfPlayersFinished; // Gets the number of players finished
}

void ACourseGameMode::SetNumberOfPlayersFinished(uint32 numPlayers) // Sets the number of players
{
	NumberOfPlayersFinished = numPlayers;
}

void ACourseGameMode::SetNumberOfPlayers(uint32 numPlayers) // Set the number of players
{
	GetGameState<AMyCourseGameState>()->NumberOfPlayers = numPlayers;
}

void ACourseGameMode::BeginPlay() 
{
	//UMultiplayerGameInstance* GameInstanceRef = Cast<UMultiplayerGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//GameInstanceRef->LoadHUDWidget();
}
