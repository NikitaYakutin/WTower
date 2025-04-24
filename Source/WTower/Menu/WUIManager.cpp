#include "WUIManager.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget/WMainMenuWidget.h"
#include "MenuWidget/WPauseMenuWidget.h"
#include "MenuWidget/WSettingsMenuWidget.h"
#include "MenuWidget/WVictoryScreenWidget.h"
#include "MenuWidget/WDefeatMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "../WTowerHUDWidget.h"
#include "../WTowerGameInstance.h"

UWUIManager::UWUIManager()
{
    // Initialize defaults
    PlayerController = nullptr;
    MainMenuWidget = nullptr;
    PauseMenuWidget = nullptr;
    SettingsMenuWidget = nullptr;
    VictoryScreenWidget = nullptr;
    DefeatMenuWidget = nullptr;
    HUDWidget = nullptr;
    CurrentMenuType = EWMenuType::None;
    bIsInGameplay = false;
}

void UWUIManager::Initialize(APlayerController* InController)
{
    PlayerController = InController;
    
    // Determine if we're in gameplay or menu level
    FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(InController);
    bIsInGameplay = (CurrentLevelName != "MainMenu");
    
    // Clear menu history
    MenuHistory.Empty();
}

void UWUIManager::SetWidgetClasses(
    TSubclassOf<UWMainMenuWidget> InMainMenuWidgetClass,
    TSubclassOf<UWPauseMenuWidget> InPauseMenuWidgetClass,
    TSubclassOf<UWSettingsMenuWidget> InSettingsMenuWidgetClass,
    TSubclassOf<UWVictoryScreenWidget> InVictoryScreenWidgetClass,
    TSubclassOf<UWDefeatMenuWidget> InDefeatMenuWidgetClass,
    TSubclassOf<UWTowerHUDWidget> InHUDWidgetClass)
{
    MainMenuWidgetClass = InMainMenuWidgetClass;
    PauseMenuWidgetClass = InPauseMenuWidgetClass;
    SettingsMenuWidgetClass = InSettingsMenuWidgetClass;
    VictoryScreenWidgetClass = InVictoryScreenWidgetClass;
    DefeatMenuWidgetClass = InDefeatMenuWidgetClass;
    HUDWidgetClass = InHUDWidgetClass;
}

void UWUIManager::ShowMenu(EWMenuType MenuType, const FString& Param)
{
    // If we're already showing this menu, do nothing
    if (CurrentMenuType == MenuType)
        return;
    
    // Record the previous menu type in history if we're not returning to None
    if (CurrentMenuType != EWMenuType::None)
    {
        MenuHistory.Add(CurrentMenuType);
    }
    
    // Hide current menu
    CloseCurrentMenu();
    
    // Show new menu based on type
    switch (MenuType)
    {
        case EWMenuType::Main:
            if (MainMenuWidgetClass)
            {
                MainMenuWidget = CreateWidgetIfNeeded<UWMainMenuWidget>(MainMenuWidgetClass, MainMenuWidget);
                if (MainMenuWidget)
                {
                    MainMenuWidget->UIManager = this;
                    MainMenuWidget->InitializeMenu();
                    MainMenuWidget->AddToViewport(100);
                    
                    // Set input mode to UI
                    FInputModeUIOnly InputMode;
                    PlayerController->SetInputMode(InputMode);
                    PlayerController->bShowMouseCursor = true;
                }
            }
            break;
            
        case EWMenuType::Pause:
            if (PauseMenuWidgetClass)
            {
                PauseMenuWidget = CreateWidgetIfNeeded<UWPauseMenuWidget>(PauseMenuWidgetClass, PauseMenuWidget);
                if (PauseMenuWidget)
                {
                    PauseMenuWidget->UIManager = this;
                    PauseMenuWidget->InitializeMenu();
                    PauseMenuWidget->AddToViewport(100);
                    
                    // Set input mode to UI but allow game input
                    FInputModeGameAndUI InputMode;
                    InputMode.SetHideCursorDuringCapture(false);
                    PlayerController->SetInputMode(InputMode);
                    PlayerController->bShowMouseCursor = true;
                    
                    // Pause the game
                    SetGamePaused(true);
                }
            }
            break;
            
        case EWMenuType::Settings:
            if (SettingsMenuWidgetClass)
            {
                SettingsMenuWidget = CreateWidgetIfNeeded<UWSettingsMenuWidget>(SettingsMenuWidgetClass, SettingsMenuWidget);
                if (SettingsMenuWidget)
                {
                    SettingsMenuWidget->UIManager = this;
                    SettingsMenuWidget->bOpenedFromPauseMenu = (MenuHistory.Num() > 0 && MenuHistory.Last() == EWMenuType::Pause);
                    SettingsMenuWidget->InitializeMenu();
                    SettingsMenuWidget->AddToViewport(100);
                    
                    // Set input mode to UI
                    FInputModeUIOnly InputMode;
                    PlayerController->SetInputMode(InputMode);
                    PlayerController->bShowMouseCursor = true;
                    SetGamePaused(true);
                }
            }
            break;
            
        case EWMenuType::Victory:
            if (VictoryScreenWidgetClass)
            {
                VictoryScreenWidget = CreateWidgetIfNeeded<UWVictoryScreenWidget>(VictoryScreenWidgetClass, VictoryScreenWidget);
                if (VictoryScreenWidget)
                {
                    VictoryScreenWidget->UIManager = this;
                    
                    // Parse the parameters (score and time)
                    TArray<FString> Params;
                    Param.ParseIntoArray(Params, TEXT("|"));
                    
                    if (Params.Num() >= 2)
                    {
                        int32 Score = FCString::Atoi(*Params[0]);
                        float Time = FCString::Atof(*Params[1]);
                        VictoryScreenWidget->SetScoreAndTime(Score, Time);
                    }
                    
                    VictoryScreenWidget->InitializeMenu();
                    VictoryScreenWidget->AddToViewport(100);
                    
                    // Set input mode to UI
                    FInputModeUIOnly InputMode;
                    PlayerController->SetInputMode(InputMode);
                    PlayerController->bShowMouseCursor = true;
                    
                    // Hide HUD
                    HideHUD();
                }
            }
            break;
            
        case EWMenuType::Defeat:
            if (DefeatMenuWidgetClass)
            {
                DefeatMenuWidget = CreateWidgetIfNeeded<UWDefeatMenuWidget>(DefeatMenuWidgetClass, DefeatMenuWidget);
                if (DefeatMenuWidget)
                {
                    DefeatMenuWidget->UIManager = this;
                    DefeatMenuWidget->SetDefeatReason(Param);
                    DefeatMenuWidget->InitializeMenu();
                    DefeatMenuWidget->AddToViewport(100);
                    
                    // Set input mode to UI
                    FInputModeUIOnly InputMode;
                    PlayerController->SetInputMode(InputMode);
                    PlayerController->bShowMouseCursor = true;
                    
                    // Hide HUD
                    HideHUD();
                }
            }
            break;
            
        case EWMenuType::HUD:
            if (HUDWidgetClass)
            {
                HUDWidget = CreateWidgetIfNeeded<UWTowerHUDWidget>(HUDWidgetClass, HUDWidget);
                if (HUDWidget)
                {
                    HUDWidget->AddToViewport(0); // Low Z-order for HUD
                    
                    // Set game-only input mode
                    FInputModeGameOnly InputMode;
                    PlayerController->SetInputMode(InputMode);
                    PlayerController->bShowMouseCursor = false;
                }
            }
            break;
            
        case EWMenuType::None:
        default:
            // Just ensure all menus are closed
            CleanupWidget(MainMenuWidget);
            CleanupWidget(PauseMenuWidget);
            CleanupWidget(SettingsMenuWidget);
            CleanupWidget(VictoryScreenWidget);
            CleanupWidget(DefeatMenuWidget);

            // Set game-only input mode
            FInputModeGameOnly InputMode;
            PlayerController->SetInputMode(InputMode);
            PlayerController->bShowMouseCursor = false;

            // Unpause game
            SetGamePaused(false);
            break;
    }
    
    // Update current menu type
    CurrentMenuType = MenuType;
}

void UWUIManager::CloseCurrentMenu()
{
    // Close the current menu widget based on type
    switch (CurrentMenuType)
    {
    case EWMenuType::Main:
        CleanupWidget(MainMenuWidget);
        break;
    case EWMenuType::Pause:
        CleanupWidget(PauseMenuWidget);
        // Unpause the game
        SetGamePaused(false);
        break;
    case EWMenuType::Settings:
        CleanupWidget(SettingsMenuWidget);
        break;
    case EWMenuType::Victory:
        CleanupWidget(VictoryScreenWidget);
        break;
    case EWMenuType::Defeat:
        CleanupWidget(DefeatMenuWidget);
        break;
    case EWMenuType::HUD:
        CleanupWidget(HUDWidget);
        break;
    case EWMenuType::None:
    default:
        // All menus are already closed
        break;
    }

    CurrentMenuType = EWMenuType::None;
}


void UWUIManager::ReturnToPreviousMenu()
{
    // If there's a menu in history, show it
    if (MenuHistory.Num() > 0)
    {
        EWMenuType PreviousMenu = MenuHistory.Last();
        MenuHistory.RemoveAt(MenuHistory.Num() - 1);
        
        // Show the previous menu
        ShowMenu(PreviousMenu);
    }
    else
    {
        // If no previous menu, just close current
        CloseCurrentMenu();
    }
}

void UWUIManager::TogglePauseMenu()
{
    // Only available in gameplay
    if (!bIsInGameplay)
        return;
    
    // If pause menu is active, close it
    if (CurrentMenuType == EWMenuType::Pause)
    {
        CloseCurrentMenu();
        
        // Show HUD if it was hidden
        ShowHUD();
        
        // Set game-only input
        FInputModeGameOnly InputMode;
        PlayerController->SetInputMode(InputMode);
        PlayerController->bShowMouseCursor = false;
        
        // Unpause the game
        SetGamePaused(false);
    }
    // If any other menu is active, do nothing
    else if (CurrentMenuType != EWMenuType::None && CurrentMenuType != EWMenuType::HUD)
    {
        return;
    }
    else
    {
        // Show pause menu
        ShowMenu(EWMenuType::Pause);
    }
}

void UWUIManager::OpenSettings()
{

    // Show settings menu
    ShowMenu(EWMenuType::Settings);
}

void UWUIManager::ShowVictoryScreen(int32 Score, float CompletionTime)
{
    // Create parameter string
    FString Param = FString::Printf(TEXT("%d|%f"), Score, CompletionTime);
    
    // Show victory screen with parameters
    ShowMenu(EWMenuType::Victory, Param);
}

void UWUIManager::ShowDefeatScreen(const FString& Reason)
{
    // Show defeat screen with reason
    ShowMenu(EWMenuType::Defeat, Reason);
}

void UWUIManager::ShowHUD()
{
    // Only in gameplay
    if (!bIsInGameplay)
        return;
    
    // If we're showing a menu, don't show HUD
    if (CurrentMenuType != EWMenuType::None && CurrentMenuType != EWMenuType::HUD)
        return;
    
    // Create and show HUD widget
    if (HUDWidgetClass)
    {
        HUDWidget = CreateWidgetIfNeeded<UWTowerHUDWidget>(HUDWidgetClass, HUDWidget);
        if (HUDWidget)
        {
            HUDWidget->AddToViewport(0);
            
            // Only set this as current if no other menu is active
            if (CurrentMenuType == EWMenuType::None)
            {
                CurrentMenuType = EWMenuType::HUD;
            }
        }
    }
}

void UWUIManager::HideHUD()
{
    // Clean up HUD widget
    CleanupWidget(HUDWidget);
    
    // Update current menu type only if HUD was the current menu
    if (CurrentMenuType == EWMenuType::HUD)
    {
        CurrentMenuType = EWMenuType::None;
    }
}

void UWUIManager::SetGamePaused(bool bPaused)
{
    if (PlayerController)
    {
        PlayerController->SetPause(bPaused);
    }
}

template<typename T>
T* UWUIManager::CreateWidgetIfNeeded(TSubclassOf<T>& WidgetClass, T*& WidgetRef)
{
    // If widget already exists, return it
    if (WidgetRef)
        return WidgetRef;
    
    // Create new widget
    if (WidgetClass && PlayerController)
    {
        WidgetRef = CreateWidget<T>(PlayerController, WidgetClass);
        return WidgetRef;
    }
    
    return nullptr;
}

template<typename T>
void UWUIManager::CleanupWidget(T*& Widget)
{
    if (Widget)
    {
        Widget->RemoveFromParent();
        Widget = nullptr;
    }
}
