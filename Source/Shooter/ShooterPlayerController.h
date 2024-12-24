#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AShooterPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
    // Input action for moving the player
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
    class UInputAction* IA_Move;

    // Input action for looking with the mouse
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
    class UInputAction* IA_Look_Mouse;

    // Input action for jumping
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
    class UInputAction* IA_Jump;

    // Input action for crouching
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
    class UInputAction* IA_Crouch;

    // Input action for sprinting
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
    class UInputAction* IA_Sprint;

    // Input mapping context for the player's controls
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
    class UInputMappingContext* IMC_Shooter;



    // Function to handle player movement
    void Move(const FInputActionValue& Value);

    // Function to handle looking with the mouse
    void LookMouse(const FInputActionValue& Value);

    // Function to handle jumping
    void Jump();

    // Function to handle crouch toggling
    void ToggleCrouch();

    // Functions to handle sprinting
    void StartSprint();

private:
    // Timer handle to reset sprint speed
    FTimerHandle SprintResetTimerHandle;

    // Declare the sprinting flag
    bool bIsSprinting;
};
