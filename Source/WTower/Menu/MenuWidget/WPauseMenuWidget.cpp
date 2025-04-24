#include "WPauseMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "../WUIManager.h"
#include <WTower/WTowerPlayerController.h>

void UWPauseMenuWidget::InitializeMenu()
{
    Super::InitializeMenu();
}

void UWPauseMenuWidget::OnResumeClicked()
{
    if (UIManager)
    {
        UIManager->CloseCurrentMenu();
        UIManager->ShowHUD();
    }
    else
    {
        // Fallback for legacy code
        AWTowerPlayerController* PC = Cast<AWTowerPlayerController>(GetOwningPlayer());
        if (PC)
        {
            PC->ClosePauseMenu();
        }
        else
        {
            RemoveFromParent();
            UGameplayStatics::SetGamePaused(this, false);
        }
    }
}

void UWPauseMenuWidget::OnSettingsClicked()
{
    if (UIManager)
    {
        UIManager->OpenSettings();
    }
    else
    {
        // Fallback for legacy code
        AWTowerPlayerController* PC = Cast<AWTowerPlayerController>(GetOwningPlayer());
        if (PC)
        {
            PC->OpenSettingsFromPause();
        }
    }
}

void UWPauseMenuWidget::OnRestartClicked()
{
    // Unpause and reload current level
    UGameplayStatics::SetGamePaused(this, false);
    UGameplayStatics::OpenLevel(this, FName(*UGameplayStatics::GetCurrentLevelName(this)));
}

void UWPauseMenuWidget::OnMainMenuClicked()
{
    // Unpause and return to main menu
    UGameplayStatics::SetGamePaused(this, false);
    UGameplayStatics::OpenLevel(this, FName("MainMenu"));
}