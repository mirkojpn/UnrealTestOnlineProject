// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"

#include "Components/Button.h"


bool UInGameMenu::Initialize()
{
	if(  Super::Initialize() == false ) {return false; }

	if( Cancel == nullptr ) {return false;}

	Cancel->OnClicked.AddDynamic(this,&UInGameMenu::OnCancelButtonDown);

	if( Quit == nullptr ) {return false;}

	Quit->OnClicked.AddDynamic(this,&UInGameMenu::OnQuitButtonDown);

	return true;
}

void UInGameMenu::OnCancelButtonDown()
{
	Teardown();
}

void UInGameMenu::OnQuitButtonDown()
{
	Teardown();

	if( MenuInterface != nullptr )
	{
		MenuInterface->LoadMainMenu();
	}	
}


