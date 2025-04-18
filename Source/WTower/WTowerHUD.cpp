#include "WTowerHUD.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"


AWTowerHUD::AWTowerHUD()
{
    // Конструктор
}
void AWTowerHUD::BeginPlay()
{
    Super::BeginPlay();

    // Проверка, установлен ли класс виджета
    if (!HUDWidgetClass)
    {
        // Загружаем класс виджета программно
        HUDWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/Blueprints/Menu/WBP_GameHUD.WBP_GameHUD_C"));
    }
    // Создаем и добавляем виджет
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (HUDWidgetClass && PC)
    {

        if (PC)
        {
            CurrentHUDWidget = CreateWidget<UUserWidget>(PC, HUDWidgetClass);
            if (CurrentHUDWidget)
            {
                CurrentHUDWidget->AddToViewport(0);
            }
        }
    }
}