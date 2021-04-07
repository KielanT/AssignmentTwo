// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDWidget.h"
#include "Components/TextBlock.h"

void UHUDWidget::Setup()
{
	this->AddToViewport();

}

void UHUDWidget::TearDown()
{
	this->RemoveFromViewport();

}

void UHUDWidget::SetMenuInterface(IMenuInterface* _MenuInterface)
{
	MenuInterface = _MenuInterface;
}

bool UHUDWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) { return false; }

	if (FinishCountText == nullptr) { return false; }

	return true;
}

void UHUDWidget::SetFinishCountText(FText Text)
{
	//if (FinishCountText != nullptr)
	//{
	//	FinishCountText->SetText(Text);
	//}
}
