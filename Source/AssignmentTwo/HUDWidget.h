// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "HUDWidget.generated.h"

class UTextBlock;

UCLASS()
class ASSIGNMENTTWO_API UHUDWidget : public UUserWidget
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
		UTextBlock* FinishCountText;

public:
	UFUNCTION()
		void SetFinishCountText(FText Text);
};
