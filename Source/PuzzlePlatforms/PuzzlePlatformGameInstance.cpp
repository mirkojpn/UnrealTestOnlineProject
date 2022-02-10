// Fill out your copyright notice in the Description page of Project Settings.
#include "PuzzlePlatformGameInstance.h"

#include "Blueprint/UserWidget.h"
#include "MainMenu/MainMenu.h"
#include "Engine/Engine.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"

const static FName SESSION_NAME = TEXT("TestSession");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

UPuzzlePlatformGameInstance::UPuzzlePlatformGameInstance(
	const FObjectInitializer& ObjectInitializer
	)
{

	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBPMainMenu"));

	if( MenuBPClass.Class == nullptr )
	{
		return;
	}

	MenuClass =
			MenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/InGameMenu/WBP_InGameMenu"));

	if( InGameMenuBPClass.Class == nullptr ) {return;}

	InGameMenuClass =
			InGameMenuBPClass.Class;
}

void UPuzzlePlatformGameInstance::Init()
{
	Super::Init();

	IOnlineSubsystem* onlineSubsystem =
						IOnlineSubsystem::Get();

	if( onlineSubsystem != nullptr )
	{
		UE_LOG(LogTemp,Warning,TEXT("Online Subsystem Loaded Correctly : %s"), *onlineSubsystem->GetSubsystemName().ToString() );
		sessionInterface =
							 onlineSubsystem->GetSessionInterface();
		if( sessionInterface.IsValid() )
		{
			UE_LOG(LogTemp,Warning,TEXT("Find Session Interface " ) );
			sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this,&UPuzzlePlatformGameInstance::OnCreateSessionComplete);
			sessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this,&UPuzzlePlatformGameInstance::OnDestroySessionComplete);
			sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this,&UPuzzlePlatformGameInstance::OnJoinSessionComplete);
		}
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Online Subsystem Load Error") );
	}

	if( GEngine != nullptr )
	{
		GEngine->OnNetworkFailure().AddUObject(this,&UPuzzlePlatformGameInstance::OnNetworkFailure );
	}
}

void UPuzzlePlatformGameInstance::LoadMenu()
{
	if( MenuClass == nullptr )
	{
		return;
	}
	Menu =
			CreateWidget<UMainMenu>(this,MenuClass );
	if( Menu == nullptr ) {return;}

	Menu->Setup();

	Menu->SetMenuInterface(this);
}

void UPuzzlePlatformGameInstance::InGameLoadMenu()
{
	if( InGameMenuClass == nullptr )
	{
		return;
	}
	InGameMenu =
			CreateWidget<UInGameMenu>(this,InGameMenuClass );
	if( InGameMenu == nullptr ) {return;}

	InGameMenu->Setup();
	
	InGameMenu->SetMenuInterface(this);	
}

void UPuzzlePlatformGameInstance::StartSession()
{
	if( sessionInterface.IsValid() )
	{
		sessionInterface->StartSession(SESSION_NAME);
	}
}

void UPuzzlePlatformGameInstance::Host(
	FString in_serverName)
{
	_serverName = in_serverName;
	if( sessionInterface.IsValid() )
	{
		FNamedOnlineSession* existingSession =
			sessionInterface->GetNamedSession(SESSION_NAME);
		if( existingSession != nullptr )
		{
			sessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}

	}
}

void UPuzzlePlatformGameInstance::Join(
	uint32		in_index
	)
{
	if( sessionInterface.IsValid() == false
		|| sessionSearch.IsValid() == false ) {return;}
	
	if( Menu != nullptr )
	{
		Menu->Teardown();
	}

	sessionInterface->JoinSession(0,SESSION_NAME,sessionSearch->SearchResults[in_index]);
	

}

void UPuzzlePlatformGameInstance::LoadMainMenu()
{
	
	APlayerController* PlayerController =
			GetFirstLocalPlayerController();

	if( PlayerController == nullptr ) { return; }

	PlayerController->ClientTravel("/Game/MenuSystem/MainMenu",ETravelType::TRAVEL_Absolute);	
}

void UPuzzlePlatformGameInstance::RefreshServerList()
{
	sessionSearch =
		MakeShareable( new FOnlineSessionSearch() );
	// sessionSearch->bIsLanQuery = true;
	sessionSearch->MaxSearchResults = 100;
	
	if( sessionSearch.IsValid() == false ) {return;}
	sessionSearch->QuerySettings.Set(
								SEARCH_PRESENCE,
								true,
								EOnlineComparisonOp::Equals
								);
	UE_LOG(LogTemp,Warning,TEXT("Start Find Session " ) );
	sessionInterface->FindSessions(0,sessionSearch.ToSharedRef() );
	sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this,&UPuzzlePlatformGameInstance::OnFindSessionComplete );
}

void UPuzzlePlatformGameInstance::OnCreateSessionComplete(
	FName sessionName,
	bool success
	)
{
	if( success == false )
	{
		UE_LOG(LogTemp,Warning,TEXT("Could Not Create a session" ) );
		return;
	}
	if( Menu != nullptr )
	{
		Menu->Teardown();
	}
	
	UEngine* engine =
			GetEngine();

	if( engine == nullptr)
	{
		return;
	}
	engine->AddOnScreenDebugMessage(0,2,FColor::Green,TEXT("Hosting"));

	UWorld* World =
			GetWorld();
	if( World == nullptr )
	{
		return;
	}
	World->ServerTravel("/Game/ThirdPersonCPP/Maps/Lobby?listen");
}

void UPuzzlePlatformGameInstance::OnDestroySessionComplete(
	FName sessionName,
	bool success
	)
{
	if( success )
	{
		CreateSession();
	}
}

void UPuzzlePlatformGameInstance::OnFindSessionComplete(
	bool success
	)
{
	UE_LOG(LogTemp,Warning,TEXT("Find Session Completed " ) );
	if( success
		&& sessionSearch.IsValid()
		&& Menu != nullptr  )
	{
		TArray<FServerData> ServerNames;
		
		for(FOnlineSessionSearchResult & searchResult : sessionSearch->SearchResults  )
		{
			UE_LOG(LogTemp,Warning,TEXT("found Session named %s " ),*searchResult.GetSessionIdStr() );
			FServerData data;
			data.Name =
				searchResult.GetSessionIdStr();
			data.MaxPlayers =
				searchResult.Session.SessionSettings.NumPublicConnections;
			data.CurrentPlayers =
				data.MaxPlayers - searchResult.Session.NumOpenPublicConnections;			
			data.HostUserName =
				searchResult.Session.OwningUserName;
			FString serverName;
			if ( searchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, serverName ) )
			{
				data.Name = serverName;
			}
			else
			{
				data.Name = "Could not get Server Name";
			}

			ServerNames.Add( data );
			
		}

		Menu->SetServerList(ServerNames);
	}
}

void UPuzzlePlatformGameInstance::OnJoinSessionComplete(
	FName in_sessionName,
	EOnJoinSessionCompleteResult::Type in_result)
{
	if( sessionInterface.IsValid() == false ) {return;}

	FString address;

	if( sessionInterface->GetResolvedConnectString(in_sessionName,address)  == false )
	{
		UE_LOG(LogTemp,Warning,TEXT("could not get connect string"));
	}
	
	UEngine* engine =
			GetEngine();
	
	if( engine == nullptr)
	{
		return;
	}
	engine->AddOnScreenDebugMessage(0,2,FColor::Green,FString::Printf(TEXT("Joining %s" ), *address));
	
	APlayerController* PlayerController =
			GetFirstLocalPlayerController();
	
	if( PlayerController == nullptr )
	{
		return;
	}
	
	PlayerController->ClientTravel(address,ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformGameInstance::OnNetworkFailure(
	UWorld* in_world,
	UNetDriver* in_driver,
	ENetworkFailure::Type in_failureTypee,
	const FString& in_errorString)
{
	LoadMainMenu();
}

void UPuzzlePlatformGameInstance::CreateSession()
{
	if( sessionInterface.IsValid() )
	{
		
		FOnlineSessionSettings sessionSettings;
		if( IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" )
		{
			sessionSettings.bIsLANMatch = true;
		}
		else
		{
			sessionSettings.bIsLANMatch = false;	
		}
		
		sessionSettings.NumPublicConnections = 5;
		sessionSettings.bShouldAdvertise = true;
		sessionSettings.bUsesPresence = true;
		sessionSettings.Set( SERVER_NAME_SETTINGS_KEY, _serverName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing );
		
		sessionInterface->CreateSession(0,SESSION_NAME,sessionSettings);		
	}
}
