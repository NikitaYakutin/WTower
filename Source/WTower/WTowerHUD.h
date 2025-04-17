#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h" 
#include "WTowerHUD.generated.h"

class UUserWidget;

/**
 * Простой HUD для отображения игровой статистики WTower
 */
UCLASS()
class WTOWER_API AWTowerHUD : public AHUD
{
    GENERATED_BODY()

public:
    // Конструктор
    AWTowerHUD();

protected:
    // Вызывается при начале игры
    virtual void BeginPlay() override;

public:
    // Основной виджет HUD
    UPROPERTY(EditDefaultsOnly, Category = "HUD")
    TSubclassOf<UUserWidget> HUDWidgetClass;

    // Ссылка на созданный виджет
    UPROPERTY()
    UUserWidget* CurrentHUDWidget;
};