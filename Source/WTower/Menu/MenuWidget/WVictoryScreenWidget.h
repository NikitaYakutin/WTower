#pragma once

#include "CoreMinimal.h"
#include "WBaseMenuWidget.h"
#include "WVictoryScreenWidget.generated.h"

/**
 * Victory screen widget displayed when player completes the game
 */
UCLASS()
class WTOWER_API UWVictoryScreenWidget : public UWBaseMenuWidget
{
    GENERATED_BODY()

public:
    virtual void InitializeMenu() override;

    // Button click handlers
    UFUNCTION(BlueprintCallable, Category = "UI")
    void OnPlayAgainClicked();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void OnMainMenuClicked();

    // Set score and completion time
    void SetScoreAndTime(int32 InScore, float InCompletionTime);

protected:
    // UI Components to be bound in Blueprint
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
    class UTextBlock* ScoreText;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
    class UTextBlock* TimeText;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
    class UTextBlock* BestScoreText;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
    class UTextBlock* BestTimeText;

private:
    // Player's score and completion time
    int32 Score;
    float CompletionTime;
    
    // Format time as MM:SS
    FString FormatTime(float TimeInSeconds) const;
};