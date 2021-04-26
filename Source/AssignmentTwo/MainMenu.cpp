// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"

void UMainMenu::Setup()
{
	this->AddToViewport();

	UWorld* World = GetWorld();

	if (World == nullptr) return;
	APlayerController* PlayerController = World->GetFirstPlayerController();

	if (PlayerController != nullptr)
	{
		PlayerController->SetInputMode(FInputModeUIOnly());
		PlayerController->bShowMouseCursor = true;
	}
}

void UMainMenu::TearDown()
{
	this->RemoveFromViewport();

	UWorld* World = GetWorld();

	if (World == nullptr) return;
	APlayerController* PlayerController = World->GetFirstPlayerController();

	if (PlayerController != nullptr)
	{
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->bShowMouseCursor = false;
	}
}

void UMainMenu::SetMenuInterface(IMenuInterface* _MenuInterface)
{
	MenuInterface = _MenuInterface;
}

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) { return false; }

	if (HostBtn == nullptr) { return false; }
	HostBtn->OnClicked.AddDynamic(this, &UMainMenu::Host);


	if (JoinMenuBtn == nullptr) { return false; }
	JoinMenuBtn->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (JoinBtn == nullptr) { return false; }
	JoinBtn->OnClicked.AddDynamic(this, &UMainMenu::Join);

	return true;
}

void UMainMenu::Host()
{
	if (MenuInterface != nullptr)
	{
		MenuInterface->Host();
	}
}

void UMainMenu::OpenJoinMenu()
{
	if (MenuSwitcher == nullptr) return;
	if (JoinMenu == nullptr) return;
	MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UMainMenu::Join()
{
	if (MenuInterface != nullptr)
	{
		FString IP = IPBox->GetText().ToString();

		if (!IP.IsEmpty())
		{
			MenuInterface->Join(IP);
		}
	}
}
