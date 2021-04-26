// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerController.h"
#include "MultiplayerGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "HUDWidget.h"
#include "Blueprint/UserWidget.h"

ACustomPlayerController::ACustomPlayerController()
{
	ConstructorHelpers::FClassFinder<UUserWidget>HUDBPClass(TEXT("/Game/Blueprints/WBP_HUD"));
	if (HUDBPClass.Class == nullptr) return;
	HUDClass = HUDBPClass.Class;
}

void ACustomPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//LoadHUDWidget();
}

void ACustomPlayerController::LoadHUDWidget()
{
	if (HUDClass == nullptr) return;
	HUD = CreateWidget<UHUDWidget>(this, HUDClass);
	if (HUD == nullptr) return;

	HUD->Setup();
}