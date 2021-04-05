// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameInstance.h"
#include "MainMenu.h"
#include "Blueprint/UserWidget.h"


UMultiplayerGameInstance::UMultiplayerGameInstance()
{
	ConstructorHelpers::FClassFinder<UUserWidget>MenuWidgetBPClass(TEXT("/Game/Blueprints/WBP_MainMenu"));
	if (MenuWidgetBPClass.Class == nullptr) return;
	MenuClass = MenuWidgetBPClass.Class;
}

void UMultiplayerGameInstance::Host()
{
	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		Menu->TearDown();
		World->ServerTravel("/Game/Levels/Lobby?listen");
	}
}

void UMultiplayerGameInstance::Join(const FString IPAddress)
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();

	if (PlayerController != nullptr)
	{
		Menu->TearDown();
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

void UMultiplayerGameInstance::LoadMenuWidget()
{
	if (!ensure(MenuClass != nullptr)) return;
	Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(Menu != nullptr)) return;

	Menu->Setup();
	Menu->SetMenuInterface(this);
}
