// Fill out your copyright notice in the Description page of Project Settings.


#include "SlidingDoorActor.h"
#include "NET/UnrealNetwork.h"

// Sets default values
ASlidingDoorActor::ASlidingDoorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

    TopPos = CreateDefaultSubobject<USceneComponent>(TEXT("Top"));
    BottomPos = CreateDefaultSubobject<USceneComponent>(TEXT("Bottom"));

    bReplicates = true;
}

// Called when the game starts or when spawned
void ASlidingDoorActor::BeginPlay()
{
	Super::BeginPlay();

    if (HasAuthority())
    {
        NetUpdateFrequency = 1;
        SetReplicates(true);
        SetReplicateMovement(true);
    }

    TargetPos = TopPos->GetComponentLocation();

}

// Called every frame
void ASlidingDoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    MultiCastSlideDoor();
}



void ASlidingDoorActor::ResetTarget_Implementation()
{
    if (TargetPos == TopPos->GetComponentLocation())
    {
        TargetPos = BottomPos->GetComponentLocation();
    }
    else if (TargetPos == BottomPos->GetComponentLocation())
    {
        TargetPos = TopPos->GetComponentLocation();
    }
    TimerTracker = 0;
}

bool ASlidingDoorActor::ResetTarget_Validate()
{
    return true;
}

void ASlidingDoorActor::MultiCastSlideDoor_Implementation()
{
    if (CurrentPos != TargetPos)
    {
        CurrentPos = FMath::VInterpConstantTo(CurrentPos, TargetPos, GetWorld()->DeltaTimeSeconds, Speed);
        Mesh->SetRelativeLocation(CurrentPos);
    }
    if (CurrentPos == TargetPos)
    {
        TimerTracker++;
        if (TimerTracker == 1)
        {
            DoorWaitTime = FMath::RandRange(1.0f, 4.0f);
            GetWorld()->GetTimerManager().SetTimer(DoorWaitTimer, this, &ASlidingDoorActor::ResetTarget, DoorWaitTime, false);
        }
    }
}

bool ASlidingDoorActor::MultiCastSlideDoor_Validate()
{
    return true;
}

void ASlidingDoorActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ASlidingDoorActor, Mesh);
    DOREPLIFETIME(ASlidingDoorActor, Speed);
    DOREPLIFETIME(ASlidingDoorActor, CurrentPos);
    DOREPLIFETIME(ASlidingDoorActor, TargetPos);
    DOREPLIFETIME(ASlidingDoorActor, TopPos);
    DOREPLIFETIME(ASlidingDoorActor, BottomPos);
    DOREPLIFETIME(ASlidingDoorActor, DoorWaitTimer);
    DOREPLIFETIME(ASlidingDoorActor, DoorWaitTime);
    DOREPLIFETIME(ASlidingDoorActor, TimerTracker);

}