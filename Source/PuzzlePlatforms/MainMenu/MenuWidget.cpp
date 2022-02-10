// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"

bool UMenuWidget::Initialize()
{
	return Super::Initialize();
}

void UMenuWidget::SetMenuInterface(
	IMainMenuInterface* in_menuInterface
	)
{
	this->MenuInterface =
			in_menuInterface;

	if( MenuInterface == nullptr )
	{
		UE_LOG(LogTemp,Error,TEXT("unable to get main menu interface.. is null"));
	}
}

void UMenuWidget::Setup()
{
	this->AddToViewport();

	UWorld* world =
			GetWorld();
	if( world == nullptr ) {return;}

	APlayerController* PlayerController =
			world->GetFirstPlayerController();

	if( PlayerController == nullptr ) {return;}

	FInputModeUIOnly inputData;
	inputData.SetWidgetToFocus(
							this->TakeWidget()
							);
	inputData.SetLockMouseToViewportBehavior(
								EMouseLockMode::DoNotLock
								);
	PlayerController->SetInputMode(
								inputData
								);
	PlayerController->bShowMouseCursor =
								true;	
}

void UMenuWidget::Teardown()
{
	this->RemoveFromViewport();

	UWorld* world =
		GetWorld();
	if( world == nullptr ) {return;}

	APlayerController* PlayerController =
			world->GetFirstPlayerController();
	if( PlayerController == nullptr ) {return;}

	FInputModeGameOnly inputData;
	
	PlayerController->SetInputMode(
								inputData
								);
	PlayerController->bShowMouseCursor =
								false;		
}
