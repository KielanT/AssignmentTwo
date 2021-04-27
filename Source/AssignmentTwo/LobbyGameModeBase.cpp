// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameModeBase.h"
#include "MultiplayerGameInstance.h"

void ALobbyGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++NumberOfPlayers; // Increments the amount of players


	if (NumberOfPlayers >= 4) // If the number of players are more or equal to 4 then move to the map
	{
		UMultiplayerGameInstance* GameInstance = Cast<UMultiplayerGameInstance>(GetGameInstance());
		GameInstance->Warp("MapOne");
	}
}

void ALobbyGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	--NumberOfPlayers;
}