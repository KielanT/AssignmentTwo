// Fill out your copyright notice in the Description page of Project Settings.


#include "SlidingDoorActor.h"
#include "NET/UnrealNetwork.h"
#include "DrawDebugHelpers.h"

// Sets default values
ASlidingDoorActor::ASlidingDoorActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(Root);
    bReplicates = true;
}

// Called when the game starts or when spawned
void ASlidingDoorActor::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        //NetUpdateFrequency = 1;
        NetUpdateFrequency = 5; // Sets the net frequency (doesn't replicate smoothly with 1)
        SetReplicates(true); // Sets the replication
        SetReplicateMovement(true); // Replicates the movement
    }

    TopPos = Root->GetRelativeLocation();
    TopPos.Z += TopOffset; // Sets the Top position

    BottomPos = Root->GetRelativeLocation();
    BottomPos.Z += BottomOffset; // Sets the botton position

    TargetPos = TopPos; // Sets the target pos to the target
    DrawDebugBox(GetWorld(), TopPos, FVector(50, 50, 50), FColor::Red, true, -1, 0, 10);
    DrawDebugBox(GetWorld(), BottomPos, FVector(50, 50, 50), FColor::Blue, true, -1, 0, 10);
}

// Called every frame
void ASlidingDoorActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    SlideDoor(); // Slides the door
    MulticastStartSlideDoor(); // Starts the sliding
}

void ASlidingDoorActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Variable replication
    DOREPLIFETIME(ASlidingDoorActor, bIsSlideDoor);
    DOREPLIFETIME(ASlidingDoorActor, TargetPos);
    DOREPLIFETIME(ASlidingDoorActor, DoorWaitTime);

}

void ASlidingDoorActor::SlideDoor()
{
    if (bIsSlideDoor) // If the door has been set to slide by multicast rpc then slide the door
    {
        if (CurrentPos.Z != TargetPos.Z)
        {
            CurrentPos = FMath::VInterpConstantTo(CurrentPos, TargetPos, GetWorld()->DeltaTimeSeconds, 500);
            Mesh->SetRelativeLocation(FVector(0, 0, CurrentPos.Z));
        }
        else
        {
            bIsSlideDoor = false; // If reached the target set the door to not slide
            TimerTracker = 0; // Reset timer tracker
        }


    }
}

void ASlidingDoorActor::MulticastStartSlideDoor_Implementation()
{
    if (!bIsSlideDoor) // If the door is not sliding then start timer
    {
        TimerTracker++; // Increments the timer tracker so that it only runs the timer once
        if (TimerTracker == 1)
        {
            DoorWaitTime = FMath::RandRange(1.0f, 4.0f); // Sets the timer to be between 1 and 4 seconds
            GetWorld()->GetTimerManager().SetTimer(DoorWaitTimer, this, &ASlidingDoorActor::MulticastUpdateSlideDoor, DoorWaitTime, false); // Starts the timer
        }
    }
}

bool ASlidingDoorActor::MulticastStartSlideDoor_Validate()
{
    return true;

}

void ASlidingDoorActor::MulticastUpdateSlideDoor_Implementation()
{
    if (HasAuthority())
    {
        if (TargetPos == TopPos) // If the target position is the top then set the target to bottom
        {
            TargetPos = BottomPos;
        }
        else if (TargetPos == BottomPos) // If the target position is the bottom then set the target to top
        {
            TargetPos = TopPos;
        }
        bIsSlideDoor = true; // Slides the door
    }
}

bool ASlidingDoorActor::MulticastUpdateSlideDoor_Validate()
{
    return true;
}