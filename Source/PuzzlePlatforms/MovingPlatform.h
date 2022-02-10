// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()


public:
	AMovingPlatform();
	UPROPERTY(EditAnywhere,Meta = (MakeEditWidget = true))
	FVector _targetLocation;

	void AddActiveTrigger();
	void RemoveActiveTrigger();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float deltatime) override;

	const float _speed = 100.0;



private:
	FVector GlobalStartLocation;
	FVector GlobalTargetLocation;

	UPROPERTY(EditAnywhere)
	int ActiveTriggers = 1;
};
