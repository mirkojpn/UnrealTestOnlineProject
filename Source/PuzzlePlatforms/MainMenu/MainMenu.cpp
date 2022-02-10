// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "UObject/ConstructorHelpers.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"

#include "ServerRow.h"
#include "Components/EditableTextBox.h"


UMainMenu::UMainMenu(
	const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));

	if( ServerRowPClass.Class == nullptr )
	{
		return;
	}
	
	_serverRowClass =
			ServerRowPClass.Class;
}

void UMainMenu::SetServerList(
	TArray<FServerData> ServerNames
	)
{
	UWorld* world =
				this->GetWorld();
	if( world == nullptr ) {return;;}

	if( ServerList == nullptr ) {return;}
	ServerList->ClearChildren();

	uint32 i = 0;
	for( const FServerData& serverData : ServerNames )
	{
		UServerRow* serverRow =
			CreateWidget<UServerRow>(world, _serverRowClass);
		if( serverRow != nullptr )
		{
			serverRow->SetServerName(
								FText::FromString(
												serverData.Name ) );
			serverRow->SetHostUser(
								FText::FromString(
												serverData.HostUserName ) );
			FString connectionFraction =
				FString::Printf(TEXT( "%d/%d" ), serverData.CurrentPlayers, serverData.MaxPlayers);
			serverRow->SetConnectionFraction(
								FText::FromString(connectionFraction));
			serverRow->Setup(this, i);
			++i;
			ServerList->AddChild(serverRow);
		}
	}
}

void UMainMenu::UpdateChildren()
{
	for (int i = 0 ; i < ServerList->GetChildrenCount(); ++i )
	{
		UServerRow* row =
			Cast<UServerRow>( ServerList->GetChildAt(i) );
		if( row != nullptr )
		{
			row->IsSelected = ( _selectedIndex.IsSet() &&_selectedIndex.GetValue() == i );
		}
	}
}

void UMainMenu::SelectIndex(
	uint32 in_index)
{
	_selectedIndex = in_index;
	UpdateChildren();
}

bool UMainMenu::Initialize()
{
	if( Super::Initialize() == false ) {return false;}

	if( Host == nullptr ) { return false; }

	Host->OnClicked.AddDynamic(
								this,
								&UMainMenu::OpenHostMenu
								);

	if( Join == nullptr ) { return false; }

	Join->OnClicked.AddDynamic(
								this,
								&UMainMenu::OnJoinMenuClick
								);
	if( Cancel == nullptr ) { return false; }

	Cancel->OnClicked.AddDynamic(
								this,
								&UMainMenu::OnCancelButtonPressed
								);

	if( JoinServer == nullptr ) { return false; }

	JoinServer->OnClicked.AddDynamic(
								this,
								&UMainMenu::OnJoinButtonClick
								);

	if( QuitButton == nullptr ) {return false;}

	QuitButton->OnClicked.AddDynamic(
								this,
								&UMainMenu::OnQuitButtonPressed
								);
	if( CancelCreation == nullptr ) { return false; }

	CancelCreation->OnClicked.AddDynamic(
								this,
								&UMainMenu::CloseHostMenu
								);

	if( CreateServer == nullptr ) { return false; }

	CreateServer->OnClicked.AddDynamic(
								this,
								&UMainMenu::OnHostButtonClick
								);
	return true;
}

void UMainMenu::OnHostButtonClick()
{
	if( MenuInterface != nullptr
		&& ServerName != nullptr )
	{
		FString serverName =
			ServerName->Text.ToString();
		MenuInterface->Host(serverName);
	}
}

void UMainMenu::OnJoinButtonClick()
{
	if( _selectedIndex.IsSet() )
	{
		UE_LOG(LogTemp,Warning,TEXT("Selected Index %d"),_selectedIndex.GetValue());
		MenuInterface->Join( _selectedIndex.GetValue() );
		
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Selected Index Not Set") );
	}
}

void UMainMenu::OnJoinMenuClick()
{
	if( MenuSwitcher == nullptr
		|| JoinMenu == nullptr 
	    || MenuInterface == nullptr){return;}
	
	MenuInterface->RefreshServerList();
	
	MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UMainMenu::OnCancelButtonPressed()
{
	if( MenuSwitcher == nullptr
		|| MainMenu == nullptr) {return;}

	MenuSwitcher->SetActiveWidget(MainMenu);	
}

void UMainMenu::OnQuitButtonPressed()
{
	UWorld* world =
			GetWorld();
	if( world == nullptr ) {return;}

	APlayerController* PlayerController =
			world->GetFirstPlayerController();

	if( PlayerController == nullptr ) {return;}

	PlayerController->ConsoleCommand("quit");
}

void UMainMenu::OpenHostMenu()
{
	if( MenuSwitcher == nullptr
		|| HostMenu == nullptr 
		|| MenuInterface == nullptr){return;}
	
	MenuSwitcher->SetActiveWidget(HostMenu);	
}

void UMainMenu::CloseHostMenu()
{
	if( MenuSwitcher == nullptr
	|| MainMenu == nullptr) {return;}

	MenuSwitcher->SetActiveWidget(MainMenu);	
}
