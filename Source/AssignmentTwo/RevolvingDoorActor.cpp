// Fill out your copyright notice in the Description page of Project Settings.


#include "RevolvingDoorActor.h"

// Sets default values
ARevolvingDoorActor::ARevolvingDoorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

// Called when the game starts or when spawned
void ARevolvingDoorActor::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		//NetUpdateFrequency = 5;
		SetReplicates(true);
		SetReplicateMovement(true);
	}
}

// Called every frame
void ARevolvingDoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

