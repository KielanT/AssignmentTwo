// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameInstance.h"
#include "MainMenu.h"
#include "HUDWidget.h"
#include "Blueprint/UserWidget.h"


UMultiplayerGameInstance::UMultiplayerGameInstance()
{
	ConstructorHelpers::FClassFinder<UUserWidget>MenuWidgetBPClass(TEXT("/Game/Blueprints/WBP_MainMenu")); // Finds the main menu widget blueprint 
	if (MenuWidgetBPClass.Class == nullptr) return;
	MenuClass = MenuWidgetBPClass.Class; // Sets the menu class

	ConstructorHelpers::FClassFinder<UUserWidget>HUDBPClass(TEXT("/Game/Blueprints/WBP_HUD"));
	if (HUDBPClass.Class == nullptr) return;
	HUDClass = HUDBPClass.Class;
}

void UMultiplayerGameInstance::Host() // Function to host (Called through the menu interface)
{
	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		Menu->TearDown(); // Removes the menu
		World->ServerTravel("/Game/Levels/Lobby?listen"); // Takes the server to the lobby
	}
}

void UMultiplayerGameInstance::Join(const FString IPAddress) // Function to join (Called through the menu interface)
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();

	if (PlayerController != nullptr)
	{
		Menu->TearDown(); // Removes menu
		PlayerController->ClientTravel(IPAddress, ETravelType::TRAVEL_Absolute); // Takes the client to the correct server
	}
}

void UMultiplayerGameInstance::Warp(const FString LevelName) /// Function to take all players to different levels
{
	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		if (HUD != nullptr)
		{
			HUD->TearDown(); // Removes hud
		}
		FString Destination = FString::Printf(TEXT("/Game/Levels/%s?listen"), *LevelName); // The travel path
		World->ServerTravel(Destination); 
	}
}

void UMultiplayerGameInstance::LoadMenuWidget() // Loads menu 
{
	if (MenuClass == nullptr) return;
	Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (Menu == nullptr) return;

	Menu->Setup(); // Adds menu to viewport
	Menu->SetMenuInterface(this); // Sets the interface 
}

