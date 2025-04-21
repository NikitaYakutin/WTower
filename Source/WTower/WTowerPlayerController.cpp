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

    // Создаём UIManager заранее, чтобы он был готов к использованию
    if (!UIManager)
    {
        UIManager = NewObject<UWUIManager>(this);
        UIManager->Initialize(this);

        // Устанавливаем классы виджетов
        UIManager->SetWidgetClasses(nullptr, PauseMenuWidgetClass,
            SettingsMenuWidgetClass, VictoryScreenWidgetClass);
    }

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
        UE_LOG(LogTemp, Warning, TEXT("WTowerPlayerController::TogglePauseMenu: Создан новый UIManager"));
    }

    // Получаем классы виджетов из GameInstance
    UWTowerGameInstance* GameInstance = Cast<UWTowerGameInstance>(GetGameInstance());
    if (GameInstance)
    {
        TSubclassOf<UWPauseMenuWidget> PauseClass;
        TSubclassOf<UWSettingsMenuWidget> SettingsClass;
        TSubclassOf<UWVictoryScreenWidget> VictoryClass;


        if (PauseClass)
        {
            UE_LOG(LogTemp, Warning, TEXT("WTowerPlayerController::TogglePauseMenu: Получен PauseMenuWidgetClass из GameInstance: %s"),
                *PauseClass->GetName());

            // Устанавливаем класс для UIManager
            UIManager->SetWidgetClasses(nullptr, PauseClass, SettingsClass, VictoryClass);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("WTowerPlayerController::TogglePauseMenu: Не удалось получить PauseMenuWidgetClass из GameInstance!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("WTowerPlayerController::TogglePauseMenu: Не удалось получить GameInstance!"));
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
