#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WTowerHUDWidget.generated.h"

class UTextBlock;
class AWTowerGameState;

/**
 * Виджет для отображения игровой статистики
 */
UCLASS()
class WTOWER_API UWTowerHUDWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    // Получить GameState
    AWTowerGameState* GetWTowerGameState() const;

    // Обновить все отображаемые значения
    void UpdateStats();

    // Форматировать время в MM:SS
    FString FormatTime(float TimeInSeconds) const;

protected:
    // Текстовые блоки для отображения статистики
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ScoreText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* TimeText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* HeightText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* MaxHeightText;
};