#include "WTowerHUDWidget.h"
#include "WTowerGameState.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Menu/WUIManager.h"

void UWTowerHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Initialize displayed values
    UpdateStats();
}

void UWTowerHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    // Update statistics every frame
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

    // Update score
    if (ScoreText)
    {
        ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), GameState->GetScore())));
    }

    // Update time
    if (TimeText)
    {
        TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %s"), *FormatTime(180.0f - (GameState->GetGameTime())))));
    }

    // Update current height
    if (HeightText)
    {
        HeightText->SetText(FText::FromString(FString::Printf(TEXT("Height: %.1f m"), GameState->GetPlayerCurrentHeight() / 100.0f)));
    }

    // Update maximum height
    if (MaxHeightText)
    {
        MaxHeightText->SetText(FText::FromString(FString::Printf(TEXT("Max Height: %.1f m"), GameState->GetPlayerMaxHeight() / 100.0f)));
    }
}

FString UWTowerHUDWidget::FormatTime(float TimeInSeconds) const
{
    int32 Minutes = FMath::FloorToInt(TimeInSeconds / 60.0f);
    int32 Seconds = FMath::FloorToInt(TimeInSeconds) % 60;

    return FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
}