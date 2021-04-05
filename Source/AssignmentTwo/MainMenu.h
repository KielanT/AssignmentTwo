// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MainMenu.generated.h"

class UButton;
class UWidgetSwitcher;
class UWidget;
class UEditableTextBox;

UCLASS()
class ASSIGNMENTTWO_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup();

	void TearDown();

	void SetMenuInterface(IMenuInterface* _MenuInterface);

protected:
	virtual bool Initialize();

	IMenuInterface* MenuInterface;

private:
	UPROPERTY(meta = (BindWidget))
		UButton* HostBtn;

	UPROPERTY(meta = (BindWidget))
		UButton* JoinMenuBtn;

	UPROPERTY(meta = (BindWidget))
		UButton* JoinBtn;

	UPROPERTY(meta = (BindWidget))
		UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
		UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
		UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
		UEditableTextBox* IPBox;

private:
	UFUNCTION()
		void Host();

	UFUNCTION()
		void OpenJoinMenu();

	UFUNCTION()
		void Join();
};
