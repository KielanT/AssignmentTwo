// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CustomPlayerController.generated.h"

class UUserWidget;
class UHUDWidget;

UCLASS()
class ASSIGNMENTTWO_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACustomPlayerController();

protected:
	virtual void BeginPlay() override; // Used for initializing when the game starts

public:
	UFUNCTION(BlueprintCallable)
		void LoadHUDWidget();

private:
	TSubclassOf<UUserWidget> HUDClass;

public:

	UHUDWidget* HUD;
};
