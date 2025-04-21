#include "MenuGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "../WTowerPlayerController.h"
#include "WUIManager.h"
#include "MenuPlayerController.h"


AMenuGameMode::AMenuGameMode()
{
    // Конструктор по умолчанию
}

// В вашем MenuGameMode.cpp
void AMenuGameMode::BeginPlay()
{
    Super::BeginPlay();

    // Получаем контроллер игрока напрямую
    APlayerController* DefaultPC = GetWorld()->GetFirstPlayerController();
    AMenuPlayerController* PC = Cast<AMenuPlayerController>(DefaultPC);
    if (PC)
    {
        // Создаем экземпляр UI Manager
        if (UIManagerClass)
        {
            UIManager = NewObject<UWUIManager>(this, UIManagerClass);
        }
        else
        {
            UIManager = NewObject<UWUIManager>(this);
        }

        // Инициализируем UI Manager с контроллером
        UIManager->Initialize(PC);

        // Устанавливаем ссылку на UIManager в контроллере для удобства доступа
        PC->SetUIManager(UIManager);

        // Показываем главное меню при запуске
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