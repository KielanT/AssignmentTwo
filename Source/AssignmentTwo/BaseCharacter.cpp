// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

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

	bReplicates = true;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Fixes issues when hit by the spinning bar
	FHitResult OutHit;
	GetCharacterMovement()->SafeMoveUpdatedComponent(FVector(0.f, 0.f, 0.01f), GetActorRotation(), true, OutHit);
	GetCharacterMovement()->SafeMoveUpdatedComponent(FVector(0.f, 0.f, -0.01f), GetActorRotation(), true, OutHit);

	CheckpointLocation = GetActorLocation(); // Sets the first checkpoint location to the players spawn/start location

}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckInFront(); // Checks if there is a player in front
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


void ABaseCharacter::MoveForwards(float axis) // Move forwards or backwards when W/S is pressed
{
	AddMovementInput(GetActorForwardVector() * axis);
}

void ABaseCharacter::Strafe(float axis) // Move Left or Right when A/D is pressed
{
	AddMovementInput(GetActorRightVector() * axis);
}

void ABaseCharacter::LookUp(float axis) // Rotates the camera to look up and down
{
	AddControllerPitchInput(axis);
}

void ABaseCharacter::Turn(float axis) // Rotates the player to turn left and right (Camera rotates with it)
{
	AddControllerYawInput(axis);
}

void ABaseCharacter::Dive() // Dive Function called with When the player right clicks
{
	ServerDive();
}

void ABaseCharacter::Push() // If a player is in front then it will push that player 
{
	ServerPush();
	
}

void ABaseCharacter::CheckInFront() // Check in front function
{
	if (IsLocallyControlled())
	{
		ServerCheckInFront();
	}
}

void ABaseCharacter::ServerCheckInFront_Implementation()
{
	FVector Location = GetActorLocation(); // Start position of the raycast
	FRotator Rotation = GetActorRotation(); // Start rotation of the raycast

	FVector End = Location - GetActorForwardVector() * CheckLength; // Sets the end position of the raycast

	FHitResult Hit; // Creates hit result

	// Collision ignores itself 
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool success = GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_Pawn, Params); // Checks if ray has hit something

	if (success) // if hit has happened
	{
		AActor* HitActor = Hit.GetActor(); // Gets the actor been hit
		
		if (HitActor != nullptr && HitActor->GetClass()->IsChildOf(ABaseCharacter::StaticClass())) // Checks if actor exists and is the correct type
		{
			isInFront = true; 
		}
	}
	else // If hit hasn't happened then set to false;
	{
		isInFront = false;
	}
}

bool ABaseCharacter::ServerCheckInFront_Validate() // Cheat protection 
{
	if (CheckLength == MAX_CHECK_LENGTH) // Only works if the check length is 200
	{
		return true;
	}
	else
	{
		return false;
	}
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

	bool success = GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_Pawn, Params); // Checks if ray has hit something
	if (success) // if hit has happened
	{
		AActor* HitActor = Hit.GetActor(); // Gets the actor been hit

		if (HitActor != nullptr) // Checks if actor exists
		{
			ABaseCharacter* OtherActor = Cast<ABaseCharacter>(HitActor);
			if (HitActor->GetClass()->IsChildOf(ABaseCharacter::StaticClass())) // Checks the actor is the correct type
			{

				if (OtherActor->isInFront) // If the player is in front then push using the launch function on the other player
				{
					OtherActor->LaunchCharacter(HitActor->GetActorForwardVector() * PushStrength, true, true);
				}
			}
		}
	}
}

bool ABaseCharacter::ServerPush_Validate()
{
	if (PushLength == MAX_PUSH_LENGTH && PushStrength <= MAX_PUSH_STRENGTH) // Only works if the push length is 100
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ABaseCharacter::ServerDive_Implementation() // Causes the player to "dive" forward
{
	LaunchCharacter(GetActorForwardVector() * DiveStrength, false, false);
}

bool ABaseCharacter::ServerDive_Validate()
{
	if (DiveStrength <= MAX_DIVE_STRENGTH) // Only runs if the Dive Strength is correct
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ABaseCharacter::ClientFinishSound_Implementation() // Plays finish sound on the client only
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FinishSound, GetActorLocation());
}