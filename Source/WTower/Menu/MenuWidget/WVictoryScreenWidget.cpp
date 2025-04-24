#include "WVictoryScreenWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "../../WTowerGameInstance.h"

void UWVictoryScreenWidget::InitializeMenu()
{
    Super::InitializeMenu();
    
    // Get game instance for best score/time
    UWTowerGameInstance* GameInstance = Cast<UWTowerGameInstance>(GetGameInstance());
    
    // Update UI with player's score and time
    if (ScoreText)
    {
        ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), Score)));
    }
    
    if (TimeText)
    {
        TimeText->SetText(FText::FromString(FString::Printf(TEXT("Completion Time: %s"), *FormatTime(CompletionTime))));
    }
    
    // Show best score and time if game instance is available
    if (GameInstance)
    {
        if (BestScoreText)
        {
            BestScoreText->SetText(FText::FromString(FString::Printf(TEXT("Best Score: %d"), GameInstance->GetBestScore())));
        }
        
        if (BestTimeText)
        {
            BestTimeText->SetText(FText::FromString(FString::Printf(TEXT("Best Time: %s"), *FormatTime(GameInstance->GetBestCompletionTime()))));
        }
    }
}

void UWVictoryScreenWidget::OnPlayAgainClicked()
{
    // Reload current level
    UGameplayStatics::OpenLevel(this, FName(*UGameplayStatics::GetCurrentLevelName(this)));
}

void UWVictoryScreenWidget::OnMainMenuClicked()
{
    // Return to main menu level
    UGameplayStatics::OpenLevel(this, FName("MainMenu"));
}

void UWVictoryScreenWidget::SetScoreAndTime(int32 InScore, float InCompletionTime)
{
    Score = InScore;
    CompletionTime = InCompletionTime;
}

FString UWVictoryScreenWidget::FormatTime(float TimeInSeconds) const
{
    int32 Minutes = FMath::FloorToInt(TimeInSeconds / 60.0f);
    int32 Seconds = FMath::FloorToInt(TimeInSeconds) % 60;

    return FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
}