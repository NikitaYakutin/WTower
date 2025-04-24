#include "MenuGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "../WTowerPlayerController.h"
#include "WUIManager.h"
#include "MenuPlayerController.h"
#include "MenuWidget/WDefeatMenuWidget.h"
#include <WTower/WTowerGameMode.h>

AMenuGameMode::AMenuGameMode()
{
    // Default constructor
}

void AMenuGameMode::BeginPlay()
{
    Super::BeginPlay();

    // Получаем контроллер игрока
    APlayerController* DefaultPC = GetWorld()->GetFirstPlayerController();
    AMenuPlayerController* PC = Cast<AMenuPlayerController>(DefaultPC);
    if (PC)
    {
        // Создаём UI Manager
        if (UIManagerClass)
        {
            UIManager = NewObject<UWUIManager>(this, UIManagerClass);
        }
        else
        {
            UIManager = NewObject<UWUIManager>(this);
        }

        // Инициализируем UIManager с контроллером
        UIManager->Initialize(PC);

        // Устанавливаем классы виджетов
        UIManager->SetWidgetClasses(
            MainMenuWidgetClass,
            PauseMenuWidgetClass,
            SettingsMenuWidgetClass,
            VictoryScreenWidgetClass,
            DefeatMenuWidgetClass,
            HUDWidgetClass
        );

        // Устанавливаем UIManager в контроллере
        PC->SetUIManager(UIManager);

        // Показываем главное меню
        UIManager->ShowMenu(EWMenuType::Main);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("MenuGameMode: No player controller available!"));
    }
}

void AMenuGameMode::CloseCurrentMenu()
{
    if (UIManager)
    {
        UIManager->CloseCurrentMenu();
    }
}

void AMenuGameMode::ReturnToPreviousMenu()
{
    if (UIManager)
    {
        UIManager->ReturnToPreviousMenu();
    }
}