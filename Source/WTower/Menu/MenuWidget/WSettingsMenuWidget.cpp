#include "WSettingsMenuWidget.h"
#include "Components/Slider.h"
#include "Components/ComboBoxString.h"
#include "Components/CheckBox.h"
#include "Kismet/GameplayStatics.h"
#include <WTower/WTowerGameInstance.h>
#include <WTower/WTowerPlayerController.h>
#include <WTower/Menu/MenuGameMode.h>

void UWSettingsMenuWidget::InitializeMenu()
{
    Super::InitializeMenu();
    
    // Get game instance
    GameInstanceRef = Cast<UWTowerGameInstance>(GetGameInstance());
    bOpenedFromPauseMenu = true;
    if (!GameInstanceRef)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get game instance in settings menu!"));
        return;
    }
    
    // Initialize available resolutions
    AvailableResolutions.Empty();
    AvailableResolutions.Add(FIntPoint(1280, 720));  // 720p
    AvailableResolutions.Add(FIntPoint(1920, 1080)); // 1080p
    AvailableResolutions.Add(FIntPoint(2560, 1440)); // 1440p
    AvailableResolutions.Add(FIntPoint(3840, 2160)); // 4K
    
    // Get current settings
    CurrentMasterVolume = GameInstanceRef->GetMasterVolume();
    CurrentMusicVolume = GameInstanceRef->GetMusicVolume();
    CurrentSFXVolume = GameInstanceRef->GetSFXVolume();
    CurrentResolution = GameInstanceRef->GetWindowSize();
    bCurrentFullscreen = GameInstanceRef->IsFullscreen();
    
    // Setup UI components if they exist
    if (MasterVolumeSlider)
    {
        MasterVolumeSlider->SetValue(CurrentMasterVolume);
    }
    
    if (MusicVolumeSlider)
    {
        MusicVolumeSlider->SetValue(CurrentMusicVolume);
    }
    
    if (SFXVolumeSlider)
    {
        SFXVolumeSlider->SetValue(CurrentSFXVolume);
    }
    
    // Setup resolution combo box
    if (ResolutionComboBox)
    {
        ResolutionComboBox->ClearOptions();
        
        int32 SelectedIndex = 0;
        for (int32 i = 0; i < AvailableResolutions.Num(); i++)
        {
            const FIntPoint& Resolution = AvailableResolutions[i];
            FString Option = FString::Printf(TEXT("%dx%d"), Resolution.X, Resolution.Y);
            ResolutionComboBox->AddOption(Option);
            
            if (Resolution == CurrentResolution)
            {
                SelectedIndex = i;
            }
        }
        
        ResolutionComboBox->SetSelectedIndex(SelectedIndex);
    }
    
    // Setup fullscreen checkbox
    if (FullscreenCheckBox)
    {
        FullscreenCheckBox->SetCheckedState(bCurrentFullscreen ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
    }
}

void UWSettingsMenuWidget::ApplySettings()
{
    if (!GameInstanceRef)
    {
        return;
    }
    
    // Apply all settings
    GameInstanceRef->SetMasterVolume(CurrentMasterVolume);
    GameInstanceRef->SetMusicVolume(CurrentMusicVolume);
    GameInstanceRef->SetSFXVolume(CurrentSFXVolume);
    GameInstanceRef->SetWindowSize(CurrentResolution);
    GameInstanceRef->SetFullscreen(bCurrentFullscreen);
    
    // Apply immediately
    GameInstanceRef->ApplyAudioSettings();
    GameInstanceRef->ApplyWindowSettings();
    // Сохраняем настройки в файл
    GameInstanceRef->SaveGameData();
    OnBackButtonClicked();

}

void UWSettingsMenuWidget::ResetToDefaults()
{
    // Reset to default values
    CurrentMasterVolume = 1.0f;
    CurrentMusicVolume = 0.7f;
    CurrentSFXVolume = 1.0f;
    CurrentResolution = FIntPoint(1280, 720);
    bCurrentFullscreen = false;
    
    // Update UI
    if (MasterVolumeSlider)
    {
        MasterVolumeSlider->SetValue(CurrentMasterVolume);
    }
    
    if (MusicVolumeSlider)
    {
        MusicVolumeSlider->SetValue(CurrentMusicVolume);
    }
    
    if (SFXVolumeSlider)
    {
        SFXVolumeSlider->SetValue(CurrentSFXVolume);
    }
    
    if (ResolutionComboBox)
    {
        ResolutionComboBox->SetSelectedIndex(0); // 720p is typically first
    }
    
    if (FullscreenCheckBox)
    {
        FullscreenCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
    }
    
    // Don't apply yet - user needs to click Apply
}

void UWSettingsMenuWidget::UpdateMasterVolume(float Value)
{
    CurrentMasterVolume = Value;
}

void UWSettingsMenuWidget::UpdateMusicVolume(float Value)
{
    CurrentMusicVolume = Value;
}

void UWSettingsMenuWidget::UpdateSFXVolume(float Value)
{
    CurrentSFXVolume = Value;
}

void UWSettingsMenuWidget::SetWindowResolution(FIntPoint Resolution)
{
    CurrentResolution = Resolution;
}

void UWSettingsMenuWidget::SetFullscreenMode(bool bFullscreen)
{
    bCurrentFullscreen = bFullscreen;
}
// В методе OnBackButtonClicked в WSettingsMenuWidget.cpp
// Метод для кнопки "Назад"
// Изменения в существующем коде

void UWSettingsMenuWidget::OnBackButtonClicked()
{
    if (UIManager)
    {
        // Если у нас есть UIManager, используем его для возврата к предыдущему меню
        UIManager->ReturnToPreviousMenu();
    }
    else
    {
        // Резервный вариант: старая логика
        AWTowerPlayerController* PC = Cast<AWTowerPlayerController>(GetOwningPlayer());
        if (PC && PC->bIsSettingsOpenFromPause)
        {
            PC->ReturnToPauseFromSettings();
        }
        else
        {
            AMenuGameMode* GameMode = Cast<AMenuGameMode>(GetWorld()->GetAuthGameMode());
            if (GameMode)
            {
                GameMode->ReturnToPreviousMenu();
            }
            else
            {
                RemoveFromParent();
            }
        }
    }
}
