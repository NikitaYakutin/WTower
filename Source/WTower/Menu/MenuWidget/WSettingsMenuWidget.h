#pragma once

#include "CoreMinimal.h"
#include "WBaseMenuWidget.h"
#include <WTower/WTowerGameInstance.h>
#include "WSettingsMenuWidget.generated.h"



/**
 * Settings menu widget for audio and window settings
 */
UCLASS()
class WTOWER_API UWSettingsMenuWidget : public UWBaseMenuWidget
{
    GENERATED_BODY()

public:
    virtual void InitializeMenu() override;
    // Метод для кнопки "Назад"
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void OnBackButtonClicked();

    // Для отслеживания предыдущего меню
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    bool bOpenedFromPauseMenu;
    // Apply and save settings
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void ApplySettings();
    
    // Reset settings to defaults
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void ResetToDefaults();
    
    // Update settings values from UI
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void UpdateMasterVolume(float Value);
    
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void UpdateMusicVolume(float Value);
    
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void UpdateSFXVolume(float Value);
    
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void SetWindowResolution(FIntPoint Resolution);
    
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void SetFullscreenMode(bool bFullscreen);

protected:
    // UI Components - to be bound in Blueprint
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Settings")
    class USlider* MasterVolumeSlider;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Settings")
    class USlider* MusicVolumeSlider;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Settings")
    class USlider* SFXVolumeSlider;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Settings")
    class UComboBoxString* ResolutionComboBox;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Settings")
    class UCheckBox* FullscreenCheckBox;
    
    // Cached game instance reference
    UPROPERTY()
    UWTowerGameInstance* GameInstanceRef;
    // Reference to the settings menu
    UPROPERTY()
    UWSettingsMenuWidget* SettingsMenu;

    // Current settings values
    float CurrentMasterVolume;
    float CurrentMusicVolume;
    float CurrentSFXVolume;
    FIntPoint CurrentResolution;
    bool bCurrentFullscreen;
    
    // Common window resolutions
    TArray<FIntPoint> AvailableResolutions;
};