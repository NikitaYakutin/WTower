#pragma once

#include "CoreMinimal.h"
#include "WBaseMenuWidget.h"
#include "WSettingsMenuWidget.h"
#include <WTower/WTowerGameInstance.h>
#include "WMainMenuWidget.generated.h"



/**
 * Main menu widget for game
 */
UCLASS()
class WTOWER_API UWMainMenuWidget : public UWBaseMenuWidget
{
    GENERATED_BODY()

public:
    virtual void InitializeMenu() override;
    
    // Функции для кнопок меню
    UFUNCTION(BlueprintCallable, Category = "Menu")
    void OnStartGameClicked();

    UFUNCTION(BlueprintCallable, Category = "Menu")
    void OnSettingsClicked();

    UFUNCTION(BlueprintCallable, Category = "Menu")
    void OnExitClicked();

protected:
    // UI Components - to be bound in Blueprint
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Menu")
    class UTextBlock* BestScoreText;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Menu")
    class UTextBlock* BestTimeText;
    

    UPROPERTY()
    UWTowerGameInstance* GameInstanceRef;
    
    // Level to load when starting the game
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Menu")
    FName GameLevelName;

};