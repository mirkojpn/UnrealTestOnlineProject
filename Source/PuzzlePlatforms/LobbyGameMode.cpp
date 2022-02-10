// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "TimerManager.h"
#include "PuzzlePlatformGameInstance.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	AddPlayerCount();
	
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	RemovePlayerCount();
}

void ALobbyGameMode::RemovePlayerCount()
{
	--_currentPlayerCount;
}

void ALobbyGameMode::StartGame()
{
	UPuzzlePlatformGameInstance* gameInstance =
								Cast<UPuzzlePlatformGameInstance>(GetGameInstance());
	if(gameInstance == nullptr ) return;

	gameInstance->StartSession();
	UWorld* World =
			GetWorld();
	if( World == nullptr )
	{
		return;
	}
	bUseSeamlessTravel = true;
	World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");	
}

void ALobbyGameMode::AddPlayerCount()
{
	++_currentPlayerCount;

	if( _currentPlayerCount >= 3 )
	{
		GetWorldTimerManager().SetTimer(_gameStartTimer,this,&ALobbyGameMode::StartGame,5);

	}
}
