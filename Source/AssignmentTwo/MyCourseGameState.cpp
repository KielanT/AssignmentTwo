// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCourseGameState.h"

AMyCourseGameState::AMyCourseGameState()
{

}

uint32 AMyCourseGameState::GetNumberOfPlayersFinished()
{
	return NumberOfPlayersFinished;
}

uint32 AMyCourseGameState::GetNumberOfPlayers()
{
	return NumberOfPlayers;
}
