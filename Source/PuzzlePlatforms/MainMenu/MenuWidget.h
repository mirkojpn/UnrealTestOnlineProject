// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainMenuInterface.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual bool Initialize() override;		
public:
	void SetMenuInterface( IMainMenuInterface* in_menuInterface );

	void Setup();
	void Teardown();
protected:
	IMainMenuInterface* MenuInterface;
};
