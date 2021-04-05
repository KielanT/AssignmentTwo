// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm")); // Creates a spring arm component to be setup
	SpringArm->SetupAttachment(RootComponent); // Sets the attachment
	SpringArm->SetRelativeRotation(FRotator(-20.f, 0.f, 0.f)); // Sets the pitch of the spring arm
	SpringArm->TargetArmLength = 300.0f; // Sets the length of the spring arm
	SpringArm->bEnableCameraLag = false; // Sets the camera lag
	//SpringArm->CameraLagSpeed = 3.0f; // Sets the speed of camera lag speed

	/*** Camera Setup ***/
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	PushStartComp = CreateDefaultSubobject<USceneComponent>(TEXT("Push Start Component"));
	PushStartComp->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	FHitResult OutHit;
	GetCharacterMovement()->SafeMoveUpdatedComponent(FVector(0.f, 0.f, 0.01f), GetActorRotation(), true, OutHit);
	GetCharacterMovement()->SafeMoveUpdatedComponent(FVector(0.f, 0.f, -0.01f), GetActorRotation(), true, OutHit);

	CheckpointLocation = GetActorLocation();

}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/*** Set Axis mapping ***/
	PlayerInputComponent->BindAxis(TEXT("MoveForwards"), this, &ABaseCharacter::MoveForwards);
	PlayerInputComponent->BindAxis(TEXT("Strafe"), this, &ABaseCharacter::Strafe);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ABaseCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ABaseCharacter::Turn);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Dive"), IE_Pressed, this, &ABaseCharacter::Dive);
	PlayerInputComponent->BindAction(TEXT("Push"), IE_Pressed, this, &ABaseCharacter::Push);
}


void ABaseCharacter::MoveForwards(float axis)
{
	AddMovementInput(GetActorForwardVector() * axis);
}

void ABaseCharacter::Strafe(float axis)
{
	AddMovementInput(GetActorRightVector() * axis);
}

void ABaseCharacter::LookUp(float axis)
{
	AddControllerPitchInput(axis);
}

void ABaseCharacter::Turn(float axis)
{
	AddControllerYawInput(axis);
}

void ABaseCharacter::Dive()
{
	ServerDive();
}

void ABaseCharacter::Push()
{
	ServerPush();
	
}

void ABaseCharacter::ServerPush_Implementation()
{
	FVector Location = PushStartComp->GetComponentLocation();
	FRotator Rotation = PushStartComp->GetComponentRotation();

	FVector End = Location + Rotation.Vector() * PushLength;

	FHitResult Hit; // Creates hit result

	// Collision ignores itself 
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool success = GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_Visibility, Params); // Checks if ray has hit something
	if (success) // if hit has happened
	{
		AActor* HitActor = Hit.GetActor(); // Gets the actor been hit
		UE_LOG(LogTemp, Warning, TEXT("Raycast Fired"));
		DrawDebugLine(GetWorld(), Location, End, FColor::Red, false, 5);

		if (HitActor != nullptr) // Checks if actor exists
		{
			UE_LOG(LogTemp, Warning, TEXT("Push"))
			ABaseCharacter* OtherActor = Cast<ABaseCharacter>(HitActor);
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("You are hitting: %s"), *HitActor->GetName()));
			if (HitActor->GetClass()->IsChildOf(ABaseCharacter::StaticClass()))
			{
				UE_LOG(LogTemp, Warning, TEXT("Push Player"));

				OtherActor->LaunchCharacter(HitActor->GetActorForwardVector() * PushStrength, false, false);
			}
		}
	}
}

bool ABaseCharacter::ServerPush_Validate()
{
	return true;
}

void ABaseCharacter::ServerDive_Implementation()
{
	LaunchCharacter(GetActorForwardVector() * DiveStrength, false, false);
}

bool ABaseCharacter::ServerDive_Validate()
{
	if (DiveStrength <= MAX_DIVE_STRENGTH)
	{
		return true;
	}
	else
	{
		return false;
	}
}