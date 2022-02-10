// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerRow.h"

#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UServerRow::SetServerName(FText in_serverName)
{
	if( ServerName != nullptr )
	{
		ServerName->SetText( in_serverName );
	}
}

void UServerRow::SetHostUser(
	FText in_hostUser)
{
	HostUser->Text = in_hostUser;
}

void UServerRow::SetConnectionFraction(
	FText in_connectionFraction)
{
	ConnectionFraction->Text = in_connectionFraction;
}

void UServerRow::Setup(
	UMainMenu* in_parent,
	uint32 in_index)
{
	_myIndex =
			in_index;
	_mainMenu=
		in_parent;
	if( RowButton != nullptr )
	{
		RowButton->OnClicked.AddDynamic(this, &UServerRow::OnRowButtonClicked);
	}
}


void UServerRow::OnRowButtonClicked()
{
	if( _mainMenu != nullptr )
	{
		_mainMenu->SelectIndex(_myIndex);
	}
}
