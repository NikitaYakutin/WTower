#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WBaseMenuWidget.h"
#include "WVictoryScreenWidget.generated.h"

UCLASS()
class WTOWER_API UWVictoryScreenWidget : public UWBaseMenuWidget
{
    GENERATED_BODY()

public:
    // Инициализация с данными о победе
    UFUNCTION(BlueprintCallable, Category = "Victory")
    void InitializeVictoryScreen(int32 FinalScore, float CompletionTime);
    
protected:
    // Компоненты UI
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
    class UTextBlock* ScoreText;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
    class UTextBlock* TimeText;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
    class UTextBlock* BestScoreText;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
    class UTextBlock* BestTimeText;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
    class UTextBlock* NewRecordText;
};