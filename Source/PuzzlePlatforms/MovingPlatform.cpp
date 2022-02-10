// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);
}


void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	if( HasAuthority() )
	{
		SetReplicates(true);
		SetReplicateMovement(true);		
	}

	GlobalStartLocation =
		GetActorLocation();

	GlobalTargetLocation =
		GetTransform().TransformPosition(_targetLocation);
}

void AMovingPlatform::Tick(float deltatime)
{
	Super::Tick(deltatime);

	if( ActiveTriggers > 0 )
	{
		if( HasAuthority() )
		{
			FVector location =
					GetActorLocation();
			const float JourneyLength =
					( GlobalTargetLocation - GlobalStartLocation ).Size();
			const float JourneyTraveled =
					( location - GlobalStartLocation ).Size();

			if( JourneyTraveled >= JourneyLength )
			{
				const FVector swap =
					GlobalStartLocation;
				GlobalStartLocation =
					GlobalTargetLocation;
				GlobalTargetLocation =
					swap;
			}
			
			const FVector Direction =
					(GlobalTargetLocation - GlobalStartLocation ).GetSafeNormal();
			
			location += _speed * deltatime * Direction;

			SetActorLocation(location);
		}		
	}
}


void AMovingPlatform::AddActiveTrigger()
{
	ActiveTriggers++;
}

void AMovingPlatform::RemoveActiveTrigger()
{
	if( ActiveTriggers > 0 )
	{
		ActiveTriggers--;
	}
}

