// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuInterface.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"

class UButton;

USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUserName;
};

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()
public:
	UMainMenu(const FObjectInitializer & ObjectInitializer);

	void SetServerList(TArray<FServerData> ServerNames);
	void UpdateChildren();

	void SelectIndex(uint32 in_index);
protected:
	
	virtual bool Initialize() override;
	
private:
	TSubclassOf<class UUserWidget> _serverRowClass;
	
	UPROPERTY(meta = (BindWidget))
	UButton* Host;

	UPROPERTY(meta = (BindWidget))
	UButton* Join;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinServer;

	UPROPERTY(meta = (BindWidget))
	UButton* Cancel;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CancelCreation;

	UPROPERTY(meta = (BindWidget))
	UButton* CreateServer;	
	
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	UWidget* HostMenu;
	
	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ServerList;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* ServerName;	
	
	//==================================

	UFUNCTION()
	void OnHostButtonClick();

	UFUNCTION()
	void OnJoinButtonClick();

	UFUNCTION()
	void OnJoinMenuClick();

	UFUNCTION()
	void OnCancelButtonPressed();

	UFUNCTION()
	void OnQuitButtonPressed();

	UFUNCTION()
	void OpenHostMenu();

	UFUNCTION()
	void CloseHostMenu();
	

	TOptional<uint32> _selectedIndex;
};
