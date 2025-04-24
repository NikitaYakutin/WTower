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

void UWMainMenuWidget::OnPlayClicked()
{
    // Open gameplay level
    UGameplayStatics::OpenLevel(this, FName("GameLevel")); // Use your actual gameplay level name
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