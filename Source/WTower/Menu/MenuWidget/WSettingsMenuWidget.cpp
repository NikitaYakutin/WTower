#include "WSettingsMenuWidget.h"
#include "Components/Slider.h"
#include "Components/ComboBoxString.h"
#include "Components/CheckBox.h"
#include "Kismet/GameplayStatics.h"
#include "../../WTowerGameInstance.h"
#include "../../WTowerPlayerController.h"
#include "../MenuGameMode.h"
#include "../WUIManager.h"

void UWSettingsMenuWidget::InitializeMenu()
{
    Super::InitializeMenu();
    
    // Get game instance
    GameInstanceRef = Cast<UWTowerGameInstance>(GetGameInstance());
    
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
// ???????? ???? ????? ??? ????????? ????????? ????? NativeConstruct
void UWSettingsMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // ???????? ????????????? ????
    InitializeMenu();

    // ??????????? ???????? ??? ???????? UI
    if (MasterVolumeSlider)
    {
        MasterVolumeSlider->OnValueChanged.AddDynamic(this, &UWSettingsMenuWidget::UpdateMasterVolume);
    }

    if (MusicVolumeSlider)
    {
        MusicVolumeSlider->OnValueChanged.AddDynamic(this, &UWSettingsMenuWidget::UpdateMusicVolume);
    }

    if (SFXVolumeSlider)
    {
        SFXVolumeSlider->OnValueChanged.AddDynamic(this, &UWSettingsMenuWidget::UpdateSFXVolume);
    }

    if (ResolutionComboBox)
    {
        ResolutionComboBox->OnSelectionChanged.AddDynamic(this, &UWSettingsMenuWidget::OnResolutionSelected);
    }

    if (FullscreenCheckBox)
    {
        FullscreenCheckBox->OnCheckStateChanged.AddDynamic(this, &UWSettingsMenuWidget::OnFullscreenCheckBoxChanged);
    }


    // ????? ????? ?????? ????? GetWidgetFromName, ???? ? ??? ??? ????????? meta = (BindWidget)
    UButton* ApplyButton = Cast<UButton>(GetWidgetFromName(TEXT("ApplyButton")));
    if (ApplyButton)
    {
        ApplyButton->OnClicked.AddDynamic(this, &UWSettingsMenuWidget::ApplySettings);
    }

    UButton* BackButton = Cast<UButton>(GetWidgetFromName(TEXT("BackButton")));
    if (BackButton)
    {
        BackButton->OnClicked.AddDynamic(this, &UWSettingsMenuWidget::OnBackButtonClicked);
    }

    UButton* ResetButton = Cast<UButton>(GetWidgetFromName(TEXT("ResetButton")));
    if (ResetButton)
    {
        ResetButton->OnClicked.AddDynamic(this, &UWSettingsMenuWidget::ResetToDefaults);
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
    
    // Save settings to disk
    GameInstanceRef->SaveGameData();
    
    // Return to previous menu
    if (UIManager)
    {
        UIManager->ReturnToPreviousMenu();
    }
    else
    {
        OnBackButtonClicked();
    }
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

void UWSettingsMenuWidget::OnBackButtonClicked()
{
    if (UIManager)
    {
        // Use UIManager to return to previous menu
        UIManager->ReturnToPreviousMenu();
    }
    else
    {
        // Fallback for legacy code
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

void UWSettingsMenuWidget::OnResolutionSelected(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    // Parse resolution from string (e.g., "1920x1080")
    TArray<FString> Dimensions;
    SelectedItem.ParseIntoArray(Dimensions, TEXT("x"), true);
    
    if (Dimensions.Num() == 2)
    {
        int32 Width = FCString::Atoi(*Dimensions[0]);
        int32 Height = FCString::Atoi(*Dimensions[1]);
        
        CurrentResolution = FIntPoint(Width, Height);
    }
}


// Обработчик для чекбокса с правильной сигнатурой
// Replace the existing OnFullscreenCheckBoxChanged function
void UWSettingsMenuWidget::OnFullscreenCheckBoxChanged(bool bIsChecked)
{
    UE_LOG(LogTemp, Log, TEXT("OnFullscreenCheckBoxChanged called with value: %s"),
        bIsChecked ? TEXT("Enabled") : TEXT("Disabled"));
    SetFullscreenMode(bIsChecked);
}

