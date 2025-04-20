// WUIManager.cpp - Единый класс управления пользовательским интерфейсом
#include "WUIManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "UI/WMainMenuWidget.h"
#include "UI/WPauseMenuWidget.h"
#include "UI/WSettingsMenuWidget.h"
#include "UI/WVictoryScreenWidget.h"
#include "UI/WDefeatScreenWidget.h"
#include "UI/WHUDWidget.h"

UWUIManager::UWUIManager()
{
    CurrentMenuType = EWMenuType::None;
    PreviousMenuType = EWMenuType::None;
    MainMenuWidget = nullptr;
    PauseMenuWidget = nullptr;
    SettingsMenuWidget = nullptr;
    VictoryScreenWidget = nullptr;
    DefeatScreenWidget = nullptr;
    HUDWidget = nullptr;
    OwningController = nullptr;
    bHUDVisible = false;
}

void UWUIManager::Initialize(APlayerController* InOwningController)
{
    OwningController = InOwningController;
}

void UWUIManager::ShowMenu(EWMenuType MenuType)
{
    if (!OwningController)
    {
        UE_LOG(LogTemp, Warning, TEXT("WUIManager: Controller not initialized!"));
        return;
    }
    
    if (MenuType == CurrentMenuType)
    {
        return; // Меню уже открыто
    }
    
    // Запоминаем предыдущее меню
    PreviousMenuType = CurrentMenuType;
    
    // Скрываем текущее меню
    HideAllMenus();
    
    // Получаем новый виджет
    UWBaseMenuWidget* NewWidget = GetOrCreateWidget(MenuType);
    if (NewWidget)
    {
        // Устанавливаем ссылку на UIManager
        NewWidget->UIManager = this;
        
        // Показываем виджет
        NewWidget->AddToViewport();
        
        // Инициализируем меню
        NewWidget->InitializeMenu();
        NewWidget->OpenMenu();
        
        // Воспроизводим анимацию открытия
        NewWidget->PlayOpenAnimation();
        
        // Настраиваем ввод для UI
        SetupUIInput();
        
        // Если это меню паузы, ставим игру на паузу
        if (MenuType == EWMenuType::Pause)
        {
            UGameplayStatics::SetGamePaused(OwningController->GetWorld(), true);
        }
    }
    
    // Обновляем текущий тип меню
    EWMenuType OldMenuType = CurrentMenuType;
    CurrentMenuType = MenuType;
    
    // Вызываем делегат
    OnMenuChanged.Broadcast(OldMenuType, CurrentMenuType);
}

void UWUIManager::CloseCurrentMenu()
{
    if (!OwningController)
    {
        UE_LOG(LogTemp, Warning, TEXT("WUIManager: Controller not initialized!"));
        return;
    }
    
    // Сохраняем текущий тип меню
    EWMenuType OldMenuType = CurrentMenuType;
    
    // Получаем текущий виджет до скрытия
    UWBaseMenuWidget* CurrentWidget = GetOrCreateWidget(CurrentMenuType);
    
    // Вызываем анимацию закрытия
    if (CurrentWidget)
    {
        CurrentWidget->PlayCloseAnimation();
    }
    
    // Скрываем все меню
    HideAllMenus();
    
    // Снимаем игру с паузы, если это меню паузы или настройки из паузы
    if (CurrentMenuType == EWMenuType::Pause || 
        (CurrentMenuType == EWMenuType::Settings && PreviousMenuType == EWMenuType::Pause))
    {
        UGameplayStatics::SetGamePaused(OwningController->GetWorld(), false);
    }
    
    // Настраиваем ввод для игры
    SetupGameInput();
    
    // Сбрасываем типы меню
    CurrentMenuType = EWMenuType::None;
    PreviousMenuType = EWMenuType::None;
    
    // Вызываем делегат
    OnMenuChanged.Broadcast(OldMenuType, CurrentMenuType);
}

void UWUIManager::ReturnToPreviousMenu()
{
    if (PreviousMenuType != EWMenuType::None)
    {
        ShowMenu(PreviousMenuType);
    }
    else
    {
        CloseCurrentMenu();
    }
}

void UWUIManager::TogglePauseMenu()
{
    if (CurrentMenuType == EWMenuType::Pause)
    {
        CloseCurrentMenu();
    }
    else if (CurrentMenuType == EWMenuType::Settings && PreviousMenuType == EWMenuType::Pause)
    {
        ReturnToPreviousMenu();
    }
    else if (CurrentMenuType == EWMenuType::None || CurrentMenuType == EWMenuType::Main)
    {
        ShowMenu(EWMenuType::Pause);
    }
}

void UWUIManager::OpenSettings()
{
    ShowMenu(EWMenuType::Settings);
}

void UWUIManager::ShowVictoryScreen()
{
    ShowMenu(EWMenuType::Victory);
}

void UWUIManager::ShowDefeatScreen()
{
    ShowMenu(EWMenuType::Defeat);
}

void UWUIManager::SetHUDVisibility(bool bShow)
{
    bHUDVisible = bShow;
    
    if (!HUDWidget && HUDWidgetClass)
    {
        HUDWidget = CreateWidget<UWHUDWidget>(OwningController, HUDWidgetClass);
    }
    
    if (HUDWidget)
    {
        if (bShow)
        {
            HUDWidget->AddToViewport();
        }
        else
        {
            HUDWidget->RemoveFromParent();
        }
    }
}

bool UWUIManager::HandleKeyPress(FKey Key)
{
    // Обработка Escape для меню паузы
    if (Key == EKeys::Escape)
    {
        TogglePauseMenu();
        return true;
    }
    
    // Возможность для текущего виджета обработать клавишу
    UWBaseMenuWidget* CurrentWidget = GetOrCreateWidget(CurrentMenuType);
    if (CurrentWidget)
    {
        return CurrentWidget->HandleKeyPress(Key);
    }
    
    return false;
}

void UWUIManager::SetupUIInput()
{
    if (OwningController)
    {
        // Показываем курсор
        OwningController->bShowMouseCursor = true;
        OwningController->bEnableClickEvents = true;
        OwningController->bEnableMouseOverEvents = true;
        
        // Настраиваем ввод для UI
        FInputModeUIOnly InputMode;
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        OwningController->SetInputMode(InputMode);
    }
}

void UWUIManager::SetupGameInput()
{
    if (OwningController)
    {
        // Скрываем курсор
        OwningController->bShowMouseCursor = false;
        OwningController->bEnableClickEvents = false;
        OwningController->bEnableMouseOverEvents = false;
        
        // Возвращаем ввод на игру
        FInputModeGameOnly InputMode;
        OwningController->SetInputMode(InputMode);
    }
}

UWBaseMenuWidget* UWUIManager::GetOrCreateWidget(EWMenuType MenuType) const
{
    switch (MenuType)
    {
    case EWMenuType::Main:
        if (!MainMenuWidget && MainMenuWidgetClass)
        {
            MainMenuWidget = CreateWidget<UWMainMenuWidget>(OwningController, MainMenuWidgetClass);
            MainMenuWidget->UIManager = const_cast<UWUIManager*>(this);
        }
        return MainMenuWidget;
        
    case EWMenuType::Pause:
        if (!PauseMenuWidget && PauseMenuWidgetClass)
        {
            PauseMenuWidget = CreateWidget<UWPauseMenuWidget>(OwningController, PauseMenuWidgetClass);
            PauseMenuWidget->UIManager = const_cast<UWUIManager*>(this);
        }
        return PauseMenuWidget;
        
    case EWMenuType::Settings:
        if (!SettingsMenuWidget && SettingsMenuWidgetClass)
        {
            SettingsMenuWidget = CreateWidget<UWSettingsMenuWidget>(OwningController, SettingsMenuWidgetClass);
            SettingsMenuWidget->UIManager = const_cast<UWUIManager*>(this);
        }
        return SettingsMenuWidget;
        
    case EWMenuType::Victory:
        if (!VictoryScreenWidget && VictoryScreenWidgetClass)
        {
            VictoryScreenWidget = CreateWidget<UWVictoryScreenWidget>(OwningController, VictoryScreenWidgetClass);
            VictoryScreenWidget->UIManager = const_cast<UWUIManager*>(this);
        }
        return VictoryScreenWidget;
        
    case EWMenuType::Defeat:
        if (!DefeatScreenWidget && DefeatScreenWidgetClass)
        {
            DefeatScreenWidget = CreateWidget<UWDefeatScreenWidget>(OwningController, DefeatScreenWidgetClass);
            DefeatScreenWidget->UIManager = const_cast<UWUIManager*>(this);
        }
        return DefeatScreenWidget;
        
    default:
        return nullptr;
    }
}

void UWUIManager::HideAllMenus()
{
    if (MainMenuWidget)
    {
        MainMenuWidget->CloseMenu();
        MainMenuWidget->RemoveFromParent();
    }
    
    if (PauseMenuWidget)
    {
        PauseMenuWidget->CloseMenu();
        PauseMenuWidget->RemoveFromParent();
    }
    
    if (SettingsMenuWidget)
    {
        SettingsMenuWidget->CloseMenu();
        SettingsMenuWidget->RemoveFromParent();
    }
    
    if (VictoryScreenWidget)
    {
        VictoryScreenWidget->CloseMenu();
        VictoryScreenWidget->RemoveFromParent();
    }
    
    if (DefeatScreenWidget)
    {
        DefeatScreenWidget->CloseMenu();
        DefeatScreenWidget->RemoveFromParent();
    }
}