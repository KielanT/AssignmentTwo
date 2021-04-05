// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckpointTriggerBox.h"
#include "BaseCharacter.h"

ACheckpointTriggerBox::ACheckpointTriggerBox()
{
	OnActorBeginOverlap.AddDynamic(this, &ACheckpointTriggerBox::OnOverlapBegin); // Binds function
}

void ACheckpointTriggerBox::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	ABaseCharacter* Player = Cast<ABaseCharacter>(OtherActor);
	Player->CheckpointLocation = GetActorLocation();
}
