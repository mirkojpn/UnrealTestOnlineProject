// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRow.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UServerRow : public UUserWidget
{
	GENERATED_BODY()
public :
	void SetServerName(FText in_serverName);
	void SetHostUser(FText in_hostUser);
	void SetConnectionFraction(FText in_connectionFraction);

	UPROPERTY(BlueprintReadOnly)
	bool IsSelected = false;
	void Setup(class UMainMenu* in_parent,uint32 in_index);
private: 
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerName;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HostUser;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ConnectionFraction;

	UPROPERTY(meta = (BindWidget))
	class UButton* RowButton;

	UFUNCTION()
	void OnRowButtonClicked();

	uint32 _myIndex = -1;

	UMainMenu* _mainMenu;

	
};
