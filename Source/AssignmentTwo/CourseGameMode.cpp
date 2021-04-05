// Fill out your copyright notice in the Description page of Project Settings.


#include "CourseGameMode.h"
#include "MyCourseGameState.h"

ACourseGameMode::ACourseGameMode()
{
	GameStateClass = AMyCourseGameState::StaticClass();
}

void ACourseGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++NumberOfPlayers;
	SetNumberOfPlayers(NumberOfPlayers);
}

void ACourseGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	--NumberOfPlayers;
	SetNumberOfPlayers(NumberOfPlayers);
}

uint32 ACourseGameMode::GetNumberOfPlayers() const
{
	return GetGameState<AMyCourseGameState>()->NumberOfPlayers;
}

uint32 ACourseGameMode::GetNumberOfPlayersFinished() const
{
	return NumberOfPlayersFinished;
}

void ACourseGameMode::SetNumberOfPlayersFinished(uint32 numPlayers)
{
	NumberOfPlayersFinished = numPlayers;
}

void ACourseGameMode::SetNumberOfPlayers(uint32 numPlayers)
{
	GetGameState<AMyCourseGameState>()->NumberOfPlayers = numPlayers;
}
