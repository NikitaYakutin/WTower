#include "WTowerHUDWidget.h"
#include "WTowerGameState.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UWTowerHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Инициализируем отображаемые значения
    UpdateStats();
}

void UWTowerHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    // Обновляем статистику каждый кадр
    UpdateStats();
}

AWTowerGameState* UWTowerHUDWidget::GetWTowerGameState() const
{
    return Cast<AWTowerGameState>(UGameplayStatics::GetGameState(GetWorld()));
}

void UWTowerHUDWidget::UpdateStats()
{
    AWTowerGameState* GameState = GetWTowerGameState();
    if (!GameState)
        return;

    // Обновляем счет
    if (ScoreText)
    {
        ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Счет: %d"), GameState->GetScore())));
    }

    // Обновляем время
    if (TimeText)
    {
        TimeText->SetText(FText::FromString(FString::Printf(TEXT("Время: %s"), *FormatTime(180.0f - (GameState->GetGameTime())))));
    }

    // Обновляем текущую высоту
    if (HeightText)
    {
        HeightText->SetText(FText::FromString(FString::Printf(TEXT("Высота: %.1f м"), GameState->GetPlayerCurrentHeight() / 100.0f)));
    }

    // Обновляем максимальную высоту
    if (MaxHeightText)
    {
        MaxHeightText->SetText(FText::FromString(FString::Printf(TEXT("Макс. высота: %.1f м"), GameState->GetPlayerMaxHeight() / 100.0f)));
    }
}

FString UWTowerHUDWidget::FormatTime(float TimeInSeconds) const
{
    int32 Minutes = FMath::FloorToInt(TimeInSeconds / 60.0f);
    int32 Seconds = FMath::FloorToInt(TimeInSeconds) % 60;

    return FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
}