#pragma once

#include "CoreMinimal.h"
#include "WBaseMenuWidget.h"
#include "WPauseMenuWidget.generated.h"

/**
 * Pause menu widget displayed when player pauses the game
 */
UCLASS()
class WTOWER_API UWPauseMenuWidget : public UWBaseMenuWidget
{
    GENERATED_BODY()

public:
    virtual void InitializeMenu() override;

    // Button click handlers
    UFUNCTION(BlueprintCallable, Category = "UI")
    void OnResumeClicked();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void OnSettingsClicked();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void OnRestartClicked();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void OnMainMenuClicked();
};