// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuInterface.h"
#include "MultiplayerGameInstance.generated.h"

class UUserWidget;
class UMainMenu;
class UHUDWidget;

UCLASS()
class ASSIGNMENTTWO_API UMultiplayerGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()


public:
	UMultiplayerGameInstance();

	UFUNCTION(Exec) // Exec to call function through console
		void Host() override;

	UFUNCTION(Exec)
		void Join(const FString IPAddress) override;

	UFUNCTION(Exec)
		void Warp(const FString LevelName);

	UFUNCTION(BlueprintCallable)
		void LoadMenuWidget();


private:
	TSubclassOf<UUserWidget> MenuClass;
	TSubclassOf<UUserWidget> HUDClass;

	UMainMenu* Menu;

public:
	UHUDWidget* HUD;
};
