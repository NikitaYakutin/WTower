#include "WTowerPlayerController.h"
#include "PlayerCharacter.h"
#include "WTowerGameState.h"
#include "GameFramework/PlayerInput.h"
#include "Engine/World.h"

AWTowerPlayerController::AWTowerPlayerController()
{
    // Set this controller to receive input
    bShowMouseCursor = false;
    bEnableClickEvents = false;
    bEnableMouseOverEvents = false;
    

}

void AWTowerPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(InPawn);
    if (PlayerCharacter)
    {
        UE_LOG(LogTemp, Log, TEXT("WTowerPlayerController: Possessed PlayerCharacter"));
    }
}

void AWTowerPlayerController::BeginPlay()
{
    Super::BeginPlay();
    // Set default controller input mode to game only
    FInputModeGameOnly InputMode;
    SetInputMode(InputMode);
    // Hide the mouse cursor by default
    SetShowMouseCursor(false);
}

void AWTowerPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    
    // All core movement and camera controls are set up in PlayerCharacter
    // No need to duplicate bindings here since your character already handles:
    // - MoveForward, MoveRight
    // - Turn, LookUp
    // - CameraZoom
    
    // You can add additional controller-specific input handling here if needed
    // For example, menu access, pause game, etc.
}
