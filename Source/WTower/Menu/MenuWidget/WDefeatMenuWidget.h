#pragma once

#include "CoreMinimal.h"
#include "WBaseMenuWidget.h"
#include "WDefeatMenuWidget.generated.h"

/**
 * Defeat menu widget displayed when player loses the game
 */
UCLASS()
class WTOWER_API UWDefeatMenuWidget : public UWBaseMenuWidget
{
    GENERATED_BODY()

public:
    virtual void InitializeMenu() override;

    // Button click handlers
    UFUNCTION(BlueprintCallable, Category = "UI")
    void OnPlayAgainClicked();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void OnMainMenuClicked();

protected:
    // UI Components to be bound in Blueprint
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
    class UTextBlock* DefeatReasonText;

    // Store the reason for defeat to display
    FString DefeatReason;

public:
    // Set the reason for defeat
    void SetDefeatReason(const FString& Reason);
};