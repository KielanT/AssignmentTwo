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

    //if (HasAuthority())
    //{
    //    NetUpdateFrequency = 1;
    //    SetReplicates(true);
    //    SetReplicateMovement(true);
    //}
    NetUpdateFrequency = 1;
    SetReplicates(true);
    SetReplicateMovement(true);

    TopPos = Root->GetRelativeLocation();
    TopPos.Z += TopOffset;

    BottomPos = Root->GetRelativeLocation();
    BottomPos.Z -= BottomOffset;

    TargetPos = TopPos;

    DrawDebugBox(GetWorld(), TopPos, FVector(50, 50, 50), FColor::Red, true, -1, 0, 10);
    DrawDebugBox(GetWorld(), BottomPos, FVector(50, 50, 50), FColor::Blue, true, -1, 0, 10);

}

// Called every frame
void ASlidingDoorActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    MulticastSlideDoor();
    UE_LOG(LogTemp, Error, TEXT("Target Pos %s"), *TargetPos.ToString());
    UE_LOG(LogTemp, Error, TEXT("Current Pos %s"), *CurrentPos.ToString());
    UE_LOG(LogTemp, Error, TEXT("Mesh Pos %s"), *Mesh->GetRelativeLocation().ToString());
}

void ASlidingDoorActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ASlidingDoorActor, Root);
    DOREPLIFETIME(ASlidingDoorActor, Mesh);
    DOREPLIFETIME(ASlidingDoorActor, TopOffset);
    DOREPLIFETIME(ASlidingDoorActor, BottomOffset);
    DOREPLIFETIME(ASlidingDoorActor, CurrentPos);
    //DOREPLIFETIME(ASlidingDoorActor, TargetPos); // Causes client to go to main menu when not commented 
    DOREPLIFETIME(ASlidingDoorActor, DoorWaitTimer);
    DOREPLIFETIME(ASlidingDoorActor, DoorWaitTime);
    DOREPLIFETIME(ASlidingDoorActor, TimerTracker);

}

void ASlidingDoorActor::MulticastSlideDoor_Implementation()
{
    if (CurrentPos.Z != TargetPos.Z)
    {
        CurrentPos = FMath::VInterpConstantTo(CurrentPos, TargetPos, GetWorld()->DeltaTimeSeconds, 500);
        Mesh->SetRelativeLocation(FVector(0, 0, CurrentPos.Z));
    }

    if (CurrentPos.Z == TargetPos.Z)
    {
        UE_LOG(LogTemp, Error, TEXT("Hit Target Pos "));
        TimerTracker++;
        if (TimerTracker == 1)
        {
            DoorWaitTime = FMath::RandRange(1.0f, 4.0f);
            GetWorld()->GetTimerManager().SetTimer(DoorWaitTimer, this, &ASlidingDoorActor::MulticastResetSlideDoor, DoorWaitTime, false);
            UE_LOG(LogTemp, Error, TEXT("Timer Started "));
        }
    }
}

bool ASlidingDoorActor::MulticastSlideDoor_Validate()
{
    if (TargetPos.Z == TopPos.Z || TargetPos.Z == BottomPos.Z)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void ASlidingDoorActor::MulticastResetSlideDoor_Implementation()
{
    if (TargetPos == TopPos)
    {
        TargetPos = BottomPos;
    }
    else if (TargetPos == BottomPos)
    {
        TargetPos = TopPos;
    }
    TimerTracker = 0;
}

bool ASlidingDoorActor::MulticastResetSlideDoor_Validate()
{
    if (TargetPos.Z == TopPos.Z || TargetPos.Z == BottomPos.Z)
    {
        return true;
    }
    else
    {
        return false;
    }
}