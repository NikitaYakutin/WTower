#include "../Menu/MenuPlayerController.h"
#include "WUIManager.h"
#include "MenuGameMode.h"

AMenuPlayerController::AMenuPlayerController()
{
    // Всегда показываем курсор мыши в меню
    bShowMouseCursor = true;

    // Включаем клики по UI элементам
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;
}

void AMenuPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // Устанавливаем режим ввода только для UI
    FInputModeUIOnly InputMode;
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    SetInputMode(InputMode);

    // Показываем курсор
    SetShowMouseCursor(true);

    // Отключаем паузу, если она была включена
    SetPause(false);

    // Проверим, есть ли у нас UIManager
    if (!UIManager)
    {
        // Попытаемся получить его из GameMode
        AMenuGameMode* MenuGameMode = Cast<AMenuGameMode>(GetWorld()->GetAuthGameMode());
        if (MenuGameMode)
        {
            UIManager = MenuGameMode->GetUIManager();

            if (!UIManager)
            {
                UE_LOG(LogTemp, Error, TEXT("MenuPlayerController: UIManager is not available from GameMode!"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("MenuPlayerController: MenuGameMode is not available!"));
        }
    }
}
void AMenuPlayerController::SetUIManager(UWUIManager* InUIManager)
{
    UIManager = InUIManager;
}

UWUIManager* AMenuPlayerController::GetUIManager() const
{
    return UIManager;
}