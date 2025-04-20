#include "WMainMenuWidget.h"
#include "WSettingsMenuWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "WBaseMenuWidget.h"
#include <WTower/Menu/MenuPlayerController.h>
#include <WTower/Menu/MenuGameMode.h>

void UWMainMenuWidget::InitializeMenu()
{
    Super::InitializeMenu();
    
    // Get game instance
    GameInstanceRef = Cast<UWTowerGameInstance>(GetGameInstance());
    
    if (GameInstanceRef)
    {
        // Update best score and time displays
        if (BestScoreText)
        {
            int32 BestScore = GameInstanceRef->GetBestScore();
            BestScoreText->SetText(FText::FromString(FString::Printf(TEXT("Best Score: %d"), BestScore)));
        }
        
        if (BestTimeText)
        {
            float BestTime = GameInstanceRef->GetBestCompletionTime();
            if (BestTime < 999999.0f)
            {
                int32 Minutes = FMath::FloorToInt(BestTime / 60.0f);
                int32 Seconds = FMath::FloorToInt(BestTime - Minutes * 60.0f);
                BestTimeText->SetText(FText::FromString(FString::Printf(TEXT("Best Time: %02d:%02d"), Minutes, Seconds)));
            }
            else
            {
                BestTimeText->SetText(FText::FromString(TEXT("Best Time: --:--")));
            }
        }
    }
    
    // Default level name if not set in editor
    if (GameLevelName.IsNone())
    {
        GameLevelName = TEXT("Challenge");
    }
}

void UWMainMenuWidget::OnStartGameClicked()
{
    AMenuGameMode* GameMode = Cast<AMenuGameMode>(GetWorld()->GetAuthGameMode());
    if (GameMode)
    {
        // Закрываем все меню и начинаем игру
        GameMode->CloseCurrentMenu();
        UGameplayStatics::OpenLevel(this, GameLevelName);
        // Здесь код для начала игры...
    }
    // Load the game level


}


void UWMainMenuWidget::OnSettingsClicked()
{
    AMenuGameMode* GameMode = Cast<AMenuGameMode>(GetWorld()->GetAuthGameMode());
    if (GameMode)
    {
        GameMode->ShowMenu(EMenuType::Settings);
    }
}
void UWMainMenuWidget::OnQuitGameClicked()
{
    // Quit the game
    if (GetOwningPlayer())
    {
        GetOwningPlayer()->ConsoleCommand("quit");
    }
}