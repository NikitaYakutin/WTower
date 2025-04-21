#include "WPauseMenuWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include <WTower/WTowerGameState.h>
#include <WTower/Menu/MenuGameMode.h>
#include <WTower/WTowerPlayerController.h>



void UWPauseMenuWidget::InitializeMenu()
{
    Super::InitializeMenu();
    
    // Default main menu level name if not set in editor
    if (MainMenuLevelName.IsNone())
    {
        MainMenuLevelName = TEXT("MainMenu");
    }
    
    // Update current score and time
    AWTowerGameState* GameState = Cast<AWTowerGameState>(UGameplayStatics::GetGameState(this));
    if (GameState)
    {
        if (CurrentScoreText)
        {
            int32 Score = GameState->GetScore();
            CurrentScoreText->SetText(FText::FromString(FString::Printf(TEXT("Current Score: %d"), Score)));
        }
        
        if (CurrentTimeText)
        {
            float GameTime = GameState->GetGameTime();
            int32 Minutes = FMath::FloorToInt(GameTime / 60.0f);
            int32 Seconds = FMath::FloorToInt(GameTime - Minutes * 60.0f);
            CurrentTimeText->SetText(FText::FromString(FString::Printf(TEXT("Current Time: %02d:%02d"), Minutes, Seconds)));
        }
    }
}

void UWPauseMenuWidget::OpenMenu()
{
    Super::OpenMenu();
    
    // Pause the game
    UGameplayStatics::SetGamePaused(this, true);
    
    // Show mouse cursor
    if (GetOwningPlayer())
    {
        GetOwningPlayer()->SetShowMouseCursor(true);
    }
}

void UWPauseMenuWidget::CloseMenu()
{
    Super::CloseMenu();
    
    // Resume the game
    UGameplayStatics::SetGamePaused(this, false);
    
    // Hide mouse cursor
    if (GetOwningPlayer())
    {
        GetOwningPlayer()->SetShowMouseCursor(false);
    }
}
// Метод для кнопки "Настройки"
void UWPauseMenuWidget::OnSettingsClicked()
{
    if (UIManager)
    {
        // Если у нас есть UIManager, используем его для открытия настроек
        UIManager->OpenSettings();
    }
    else
    {
        // Резервный вариант: старая логика
        AWTowerPlayerController* PC = Cast<AWTowerPlayerController>(GetOwningPlayer());
        if (PC)
        {
            PC->OpenSettingsFromPause();
        }
    }
}

void UWPauseMenuWidget::OnResumeClicked()
{
    if (UIManager)
    {
        // Если у нас есть UIManager, используем его для закрытия меню паузы
        UIManager->CloseCurrentMenu();
    }
    else
    {
        // Резервный вариант: старая логика
        AWTowerPlayerController* PC = Cast<AWTowerPlayerController>(GetOwningPlayer());
        if (PC)
        {
            PC->ClosePauseMenu();
        }
    }
}

void UWPauseMenuWidget::OnMainMenuClicked()
{
    if (UIManager)
    {
        // Если у нас есть UIManager, используем его для закрытия меню
        UIManager->CloseCurrentMenu();
    }
    else
    {
        // Резервный вариант: старая логика
        AWTowerPlayerController* PC = Cast<AWTowerPlayerController>(GetOwningPlayer());
        if (PC)
        {
            PC->ClosePauseMenu();
        }
    }

    // Переходим на уровень с главным меню
    UGameplayStatics::OpenLevel(this, MainMenuLevelName);
}