#pragma once

#include "CoreMinimal.h"
#include "WBaseMenuWidget.h"
#include "WMainMenuWidget.generated.h"

/**
 * Main menu widget displayed at game start
 */
UCLASS()
class WTOWER_API UWMainMenuWidget : public UWBaseMenuWidget
{
    GENERATED_BODY()

public:
    virtual void InitializeMenu() override;

    // Button click handlers
    UFUNCTION(BlueprintCallable, Category = "UI")
    void OnPlayClicked();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void OnSettingsClicked();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void OnQuitClicked();

protected:
    // UI Components to be bound in Blueprint
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
    class UTextBlock* BestScoreText;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
    class UTextBlock* BestTimeText;

private:
    // Format time as MM:SS
    FString FormatTime(float TimeInSeconds) const;
};