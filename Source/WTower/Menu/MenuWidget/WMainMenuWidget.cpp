#include "WMainMenuWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "../../WTowerGameInstance.h"
#include "../WUIManager.h"

void UWMainMenuWidget::InitializeMenu()
{
    Super::InitializeMenu();

    // Get game instance for best score/time
    UWTowerGameInstance* GameInstance = Cast<UWTowerGameInstance>(GetGameInstance());

    // Show best score and time if game instance is available and has valid records
    if (GameInstance)
    {
        // Проверяем, есть ли у пользователя реальные результаты
        int32 BestScore = GameInstance->GetBestScore();
        float BestTime = GameInstance->GetBestCompletionTime();

        if (BestScoreText)
        {
            // Отображаем лучший счет только если он больше нуля
            if (BestScore > 0)
            {
                BestScoreText->SetText(FText::FromString(FString::Printf(TEXT("Best Score: %d"), BestScore)));
                BestScoreText->SetVisibility(ESlateVisibility::Visible);
            }
            else
            {
                // Скрываем текстовое поле если результатов нет
                BestScoreText->SetVisibility(ESlateVisibility::Hidden);
            }
        }

        if (BestTimeText)
        {
            // Отображаем лучшее время только если оно меньше значения по умолчанию
            // (в коде уже видно, что значение по умолчанию примерно 999990.0f)
            if (BestTime < 999990.0f)
            {
                BestTimeText->SetText(FText::FromString(FString::Printf(TEXT("Best Time: %s"), *FormatTime(BestTime))));
                BestTimeText->SetVisibility(ESlateVisibility::Visible);
            }
            else
            {
                // Скрываем текстовое поле если результатов нет
                BestTimeText->SetVisibility(ESlateVisibility::Hidden);
            }
        }
    }
}

void UWMainMenuWidget::OnPlayClicked()
{
    // Open gameplay level
    UGameplayStatics::OpenLevel(this, "Challenge"); // Use your actual gameplay level name
}

void UWMainMenuWidget::OnSettingsClicked()
{
    if (UIManager)
    {
        UIManager->OpenSettings();
    }
}

void UWMainMenuWidget::OnQuitClicked()
{
    // Quit the game
    APlayerController* PC = GetOwningPlayer();
    if (PC)
    {
        PC->ConsoleCommand("quit");
    }
}

FString UWMainMenuWidget::FormatTime(float TimeInSeconds) const
{
    // Handle the default large value in the game instance
    if (TimeInSeconds >= 999990.0f)
    {
        return TEXT("--:--");
    }
    
    int32 Minutes = FMath::FloorToInt(TimeInSeconds / 60.0f);
    int32 Seconds = FMath::FloorToInt(TimeInSeconds) % 60;

    return FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
}