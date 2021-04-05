// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameModeBase.h"
#include "MultiplayerGameInstance.h"

void ALobbyGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++NumberOfPlayers;


	if (NumberOfPlayers >= 4)
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