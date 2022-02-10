// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformTrigger.h"
#include "Components/BoxComponent.h"

#include "MovingPlatform.h"

// Sets default values
APlatformTrigger::APlatformTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerVolume =
			CreateDefaultSubobject<UBoxComponent>(FName("TriggerVolume"));
	if( TriggerVolume == nullptr )
	{
		return;
	}
	RootComponent =
			TriggerVolume;


}

// Called when the game starts or when spawned
void APlatformTrigger::BeginPlay()
{
	Super::BeginPlay();
	if( TriggerVolume == nullptr ){return;}
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this,&APlatformTrigger::OnOverlapBegin);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this,&APlatformTrigger::OnOverlapEnd);	
}

// Called every frame
void APlatformTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlatformTrigger::OnOverlapBegin(
	UPrimitiveComponent*	OverlappedComp,
	AActor*					OtherActor,
	UPrimitiveComponent*	OtherComp,
	int32					OtherBodyIndex,
	bool					bFromSweep,
	const FHitResult&		SweepResult)
{

	for( AMovingPlatform* Platform : PlatformsToTrigger )
	{
		Platform->AddActiveTrigger();
	}
	UE_LOG(LogTemp,Warning,TEXT("Begin Overlap"));
}

void APlatformTrigger::OnOverlapEnd(
	UPrimitiveComponent*	OverlappedComp,
	AActor*					OtherActor,
	UPrimitiveComponent*	OtherComp,
	int32					OtherBodyIndex)
{
	for( AMovingPlatform* Platform : PlatformsToTrigger )
	{
		Platform->RemoveActiveTrigger();
	}
	UE_LOG(LogTemp,Warning,TEXT("End Overlap"));
}

