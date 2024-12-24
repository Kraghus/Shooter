#include "ShooterPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Pawn.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

AShooterPlayerController::AShooterPlayerController()
{
    // Constructor code, if needed
    bIsSprinting = false;
}

void AShooterPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // Get the enhanced input local player subsystem
    if (APlayerController* PC = Cast<APlayerController>(this))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            // Add the input mapping context
            Subsystem->AddMappingContext(IMC_Shooter, 1);
        }
    }
}

void AShooterPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // Ensure the input component is of type UEnhancedInputComponent
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
    {
        // Bind the Move action to the Move function
        EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AShooterPlayerController::Move);

        // Bind the LookMouse action to the LookMouse function
        EnhancedInputComponent->BindAction(IA_Look_Mouse, ETriggerEvent::Triggered, this, &AShooterPlayerController::LookMouse);

        // Bind the Jump action to the Jump function
        EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &AShooterPlayerController::Jump);

        // Bind the Crouch action to the ToggleCrouch function
        EnhancedInputComponent->BindAction(IA_Crouch, ETriggerEvent::Triggered, this, &AShooterPlayerController::ToggleCrouch);

        // Bind the Sprint action to the sprint function
        EnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Started, this, &AShooterPlayerController::StartSprint);
    }
}

void AShooterPlayerController::Move(const FInputActionValue& Value)
{
    // Extract the 2D movement vector from the input value
    FVector2D MovementVector = Value.Get<FVector2D>();

    // Get the forward and right directions based on the control rotation
    FRotator CurrentControlRotation = GetControlRotation();
    FRotator YawRotation(0, ControlRotation.Yaw, 0);

    // Get forward and right vector from yaw rotation
    FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    if (APawn* ControlledPawn = GetPawn())
    {
        // Add movement input in the direction the player is facing
        ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
        ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
    }
}

void AShooterPlayerController::LookMouse(const FInputActionValue& Value)
{
    // Extract the 2D look vector from the input value
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    // Get the character pawn to control rotation
    APawn* ControlledPawn = GetPawn();
    if (ControlledPawn)
    {
        ACharacter* ControlledCharacter = Cast<ACharacter>(ControlledPawn);
        if (ControlledCharacter)
        {
            // Determine if the character is moving
            FVector Velocity = ControlledCharacter->GetVelocity();
            bool bIsMoving = Velocity.SizeSquared() > 0.0f;

            if (bIsMoving)
            {
                // Steer the character while moving
                AddYawInput(LookAxisVector.X);
            }

            // Rotate the camera around the character (pitch) regardless of movement
            AddYawInput(LookAxisVector.X);
            AddPitchInput(LookAxisVector.Y);
        }
    }
}

void AShooterPlayerController::Jump()
{
    // Get the character pawn to trigger jump
    APawn* ControlledPawn = GetPawn();
    if (ControlledPawn)
    {
        ACharacter* ControlledCharacter = Cast<ACharacter>(ControlledPawn);
        if (ControlledCharacter)
        {
            // Make the character jump
            ControlledCharacter->Jump();
        }
    }
}

void AShooterPlayerController::ToggleCrouch()
{
    // Get the character pawn to trigger crouch or uncrouch
    APawn* ControlledPawn = GetPawn();
    if (ControlledPawn)
    {
        ACharacter* ControlledCharacter = Cast<ACharacter>(ControlledPawn);
        if (ControlledCharacter)
        {
            if (ControlledCharacter->bIsCrouched)
            {
                // Make the character stop crouching
                ControlledCharacter->UnCrouch();
            }
            else
            {
                // Make the character crouch
                ControlledCharacter->Crouch();
            }
        }
    }
}

void AShooterPlayerController::StartSprint()
{
    // Check if the character is already sprinting
    if (bIsSprinting)
    {
        return;
    }

    // Get the character pawn to start sprinting
    APawn* ControlledPawn = GetPawn();
    if (ControlledPawn)
    {
        ACharacter* ControlledCharacter = Cast<ACharacter>(ControlledPawn);
        if (ControlledCharacter)
        {
            // Store the default walk speed
            float DefaultWalkSpeed = ControlledCharacter->GetCharacterMovement()->MaxWalkSpeed;

            // Increase the character's max walk speed for sprinting
            ControlledCharacter->GetCharacterMovement()->MaxWalkSpeed *= 1.5f; // Example multiplier

            // Set sprinting flag
            bIsSprinting = true;

            // Capture the necessary variables explicitly
            auto& TimerManager = GetWorld()->GetTimerManager();
            FTimerHandle& TimerHandle = SprintResetTimerHandle;

            // Check the character's velocity periodically
            TimerManager.SetTimer(TimerHandle, [this, ControlledCharacter, DefaultWalkSpeed, &TimerManager, &TimerHandle]()
                {
                    // Get the current velocity of the character
                    FVector Velocity = ControlledCharacter->GetVelocity();

                    // If the velocity drops below the default walk speed, reset the max walk speed
                    if (Velocity.Size() < DefaultWalkSpeed)
                    {
                        ControlledCharacter->GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
                        TimerManager.ClearTimer(TimerHandle);
                        bIsSprinting = false; // Reset sprinting flag
                    }

                }, 0.1f, true);
        }
    }
}