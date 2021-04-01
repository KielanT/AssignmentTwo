// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameInstance.h"

void UMultiplayerGameInstance::Host()
{
	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		World->ServerTravel("/Game/Levels/Lobby?listen");
	}
}

void UMultiplayerGameInstance::Join(const FString IPAddress)
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();

	if (PlayerController != nullptr)
	{
		PlayerController->ClientTravel(IPAddress, ETravelType::TRAVEL_Absolute);
	}
}

void UMultiplayerGameInstance::Warp(const FString LevelName)
{
	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		FString Destination = FString::Printf(TEXT("/Game/Levels/%s?listen"), *LevelName);
		World->ServerTravel(Destination);
	}
}
