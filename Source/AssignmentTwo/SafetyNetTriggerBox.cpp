// Fill out your copyright notice in the Description page of Project Settings.


#include "SafetyNetTriggerBox.h"
#include "BaseCharacter.h"

ASafetyNetTriggerBox::ASafetyNetTriggerBox()
{
	OnActorBeginOverlap.AddDynamic(this, &ASafetyNetTriggerBox::OnOverlapBegin); // Binds function
}

void ASafetyNetTriggerBox::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	ABaseCharacter* Player = Cast<ABaseCharacter>(OtherActor);
	Player->SetActorLocation(Player->CheckpointLocation); // Sets the players position when the player falls off the world
}
