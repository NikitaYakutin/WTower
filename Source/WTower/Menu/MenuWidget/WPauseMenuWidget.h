#pragma once

#include "CoreMinimal.h"
#include "WSettingsMenuWidget.h"
#include "WBaseMenuWidget.h"
#include "WPauseMenuWidget.generated.h"


/**
 * Pause menu widget for in-game pausing
 */
UCLASS()
class WTOWER_API UWPauseMenuWidget : public UWBaseMenuWidget
{
    GENERATED_BODY()

public:
    virtual void InitializeMenu() override;
    virtual void OpenMenu() override;
    virtual void CloseMenu() override;
    
    // Button handlers
    UFUNCTION(BlueprintCallable, Category = "Menu")
    void OnResumeClicked();
    
    UFUNCTION(BlueprintCallable, Category = "Menu")
    void OnSettingsClicked();
    
    UFUNCTION(BlueprintCallable, Category = "Menu")
    void OnMainMenuClicked();

protected:
    // UI Components - to be bound in Blueprint
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Menu")
    class UTextBlock* CurrentScoreText;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Menu")
    class UTextBlock* CurrentTimeText;
    
    // Settings menu widget class
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Menu")
    TSubclassOf<UWSettingsMenuWidget> SettingsMenuClass;
    
    // Reference to the settings menu
    UPROPERTY()
    UWBaseMenuWidget* SettingsMenu;
    
    // Main menu level name
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Menu")
    FName MainMenuLevelName;
};