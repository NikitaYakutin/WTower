#include "../Menu/MenuPlayerController.h"

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
}