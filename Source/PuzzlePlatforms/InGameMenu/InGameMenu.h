// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../MainMenu/MainMenuInterface.h"
#include "../MainMenu/MainMenu.h"
#include "InGameMenu.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UInGameMenu : public UMenuWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
	UButton* Cancel;

	UPROPERTY(meta = (BindWidget))
	UButton* Quit;

private:
	UFUNCTION()
	void OnCancelButtonDown();

	UFUNCTION()
	void OnQuitButtonDown();
	
	
};
