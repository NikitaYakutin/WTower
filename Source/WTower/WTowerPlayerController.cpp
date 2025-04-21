#include "WTowerPlayerController.h"
#include "PlayerCharacter.h"
#include "WTowerGameState.h"
#include "GameFramework/PlayerInput.h"
#include "Engine/World.h"
#include "Menu/MenuWidget/WPauseMenuWidget.h"
#include <Kismet/GameplayStatics.h>
#include "Menu/MenuGameMode.h"

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
     // Add pause game key binding
    InputComponent->BindAction("TogglePause", IE_Pressed, this, &AWTowerPlayerController::TogglePauseMenu);
}
// Реализация TogglePauseMenu
void AWTowerPlayerController::TogglePauseMenu()
{
    // Проверяем, есть ли у нас UI Manager
    if (!UIManager)
    {
        UIManager = NewObject<UWUIManager>(this);
        UIManager->Initialize(this);
    }

    // Используем UI Manager для переключения меню паузы
    UIManager->TogglePauseMenu();
}

// Добавляем дополнительные методы для работы с меню
void AWTowerPlayerController::OpenSettingsFromPause()
{
    if (UIManager)
    {
        UIManager->OpenSettings();
    }
}

void AWTowerPlayerController::ReturnToPauseFromSettings()
{
    if (UIManager)
    {
        UIManager->ReturnToPreviousMenu();
    }
}

void AWTowerPlayerController::ClosePauseMenu()
{
    if (UIManager)
    {
        UIManager->CloseCurrentMenu();
    }
}
