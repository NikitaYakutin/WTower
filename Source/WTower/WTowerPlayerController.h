#pragma once

#include "CoreMinimal.h"
#include "Menu/MenuWidget/WSettingsMenuWidget.h"
#include "Menu/MenuWidget/WPauseMenuWidget.h"
#include "GameFramework/PlayerController.h"
#include "Menu/WUIManager.h"
#include "WTowerPlayerController.generated.h"


/**
 * Custom player controller for WTower
 */
UCLASS()
class WTOWER_API AWTowerPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    // Constructor
    AWTowerPlayerController();
    // Toggle pause menu
// Добавьте в заголовок

        // Toggle pause menu
    void TogglePauseMenu();
    // Добавьте метод:
    UFUNCTION(BlueprintCallable, Category = "UI")
    void SetUIManager(UWUIManager* InUIManager);
    // Functions to manage menu navigation
    UFUNCTION(BlueprintCallable, Category = "UI")
    void OpenSettingsFromPause();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void ReturnToPauseFromSettings();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void ClosePauseMenu();

    // Flag to track if settings menu was opened from pause menu
    UPROPERTY()
    bool bIsSettingsOpenFromPause;

    // Get the UI Manager
    UFUNCTION(BlueprintCallable, Category = "UI")
    UWUIManager* GetUIManager() const { return UIManager; }


    // Called when the controller possesses a pawn
    virtual void OnPossess(APawn* InPawn) override;
    
    // Called when game starts
    virtual void BeginPlay() override;

protected:
    // Called to bind functionality to input
    virtual void SetupInputComponent() override;
    // Добавьте эти свойства в protected или public секцию
    // Классы виджетов для UI
    UPROPERTY(EditDefaultsOnly, Category = "UI|Widgets")
    TSubclassOf<class UWPauseMenuWidget> PauseMenuWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI|Widgets")
    TSubclassOf<class UWSettingsMenuWidget> SettingsMenuWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI|Widgets")
    TSubclassOf<class UWVictoryScreenWidget> VictoryScreenWidgetClass;
    UPROPERTY(EditDefaultsOnly, Category = "UI|Widgets")
    TSubclassOf<class UWMainMenuWidget> MainMenuWidgetClass;
    // UI Manager reference
    UPROPERTY()
    UWUIManager* UIManager;

};