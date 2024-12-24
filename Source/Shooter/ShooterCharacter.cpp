// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Create the SpringArmComponent and attach it to the character's root component.
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);

    // The length of the spring arm (how far the camera will be from the character).
    SpringArm->TargetArmLength = 500.0f;
    // Enable the arm to rotate relative to the character
    SpringArm->bUsePawnControlRotation = true;

    // Set the height of the spring arm (and thus the camera) relative to the character.
    SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f)); // Adjust the Z value to set the desired height.


    // Create the CameraComponent and attach it to the end of the spring arm.
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

    // Disable camera rotation relative to the arm. We want the arm to handle this.
    Camera->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

