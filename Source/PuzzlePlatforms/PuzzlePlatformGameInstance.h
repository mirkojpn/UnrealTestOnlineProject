// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MainMenu/MainMenuInterface.h"
#include "InGameMenu/InGameMenu.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "PuzzlePlatformGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UPuzzlePlatformGameInstance : public UGameInstance, public IMainMenuInterface
{
	GENERATED_BODY()
	UPuzzlePlatformGameInstance( const FObjectInitializer & ObjectInitializer );

public:
	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void LoadMenu();
	
	UFUNCTION(BlueprintCallable)
	void InGameLoadMenu();

	void StartSession();

	UFUNCTION(Exec)
	void Host(FString in_serverName) override;

	UFUNCTION(Exec)
	void Join(uint32 in_index) override;

	

	UFUNCTION()
	void LoadMainMenu() override;

	void RefreshServerList() override;
private:
	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> InGameMenuClass;

	class UMainMenu* Menu;
	UInGameMenu* InGameMenu;
	

	IOnlineSessionPtr sessionInterface;
	TSharedPtr< class FOnlineSessionSearch > sessionSearch;
	
	void OnCreateSessionComplete(FName sessionName, bool success);
	void OnDestroySessionComplete(FName sessionName, bool success);
	void OnFindSessionComplete( bool success );
	void OnJoinSessionComplete(FName in_sessionName,EOnJoinSessionCompleteResult::Type in_result);

	UFUNCTION()
	void OnNetworkFailure(UWorld* in_world,UNetDriver* in_driver, ENetworkFailure::Type in_failureTypee, const FString& in_errorString );
	
	FString _serverName;
	void CreateSession();
};
