#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h" 
#include "Menu/WUIManager.h"
#include "Menu/MenuWidget/WMainMenuWidget.h"
#include "Menu/MenuWidget/WPauseMenuWidget.h"
#include "Menu/MenuWidget/WSettingsMenuWidget.h"
#include "Menu/MenuWidget/WVictoryScreenWidget.h"
#include "Menu/MenuWidget/WDefeatMenuWidget.h"
#include "WTowerGameMode.generated.h"

/**
 * Basic GameMode for WTower that spawns the player character at PlayerStart locations
 */
UCLASS()
class WTOWER_API AWTowerGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    // Constructor
    AWTowerGameMode();
    
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    

    // Override FindPlayerStart to customize player spawning (if needed)
    virtual AActor* FindPlayerStart_Implementation(AController* Player, const FString& IncomingName) override;

// ===============RuleGame===============
   // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Обработчики событий для условий победы и поражения
    UFUNCTION()
    void OnGameCompleted(float CompletionTime);

    // Методы для завершения игры
    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void EndGameWithVictory();

    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void EndGameWithDefeat(FString Reason);

    // Настройка игры
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Settings")
    float GameTimeLimit;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Settings")
    float MinimumSafeHeight;

    // Делегаты для событий окончания игры
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameOver, bool, bIsVictory, FString, Reason);

    // Публичный делегат для оповещения о окончании игры
    UPROPERTY(BlueprintAssignable, Category = "Game Flow")
    FOnGameOver OnGameOver;
//=======================================

    // Update best score and time when game ends
    void UpdateBestScoreAndTime();
    // Получить UIManager
    UFUNCTION(BlueprintCallable, Category = "UI")
    UWUIManager* GetUIManager() const { return UIManager; }

    // Методы для управления меню
    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowMenu(EWMenuType MenuType);

    UFUNCTION(BlueprintCallable, Category = "UI")
    void CloseCurrentMenu();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void ReturnToPreviousMenu();

    // Классы виджетов для UI
    UPROPERTY(EditDefaultsOnly, Category = "UI|Widgets")
    TSubclassOf<class UWMainMenuWidget> MainMenuWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI|Widgets")
    TSubclassOf<class UWPauseMenuWidget> PauseMenuWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI|Widgets")
    TSubclassOf<class UWSettingsMenuWidget> SettingsMenuWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI|Widgets")
    TSubclassOf<class UWVictoryScreenWidget> VictoryScreenWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI|Widgets")
    TSubclassOf<class UWDefeatMenuWidget> DefeatMenuWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI|Widgets")
    TSubclassOf<class UWTowerHUDWidget> HUDWidgetClass;
    // UIManager для централизованного управления UI

    UPROPERTY(EditDefaultsOnly, Category = "UI|Widgets")
    TSubclassOf<class UWUIManager> UIManagerClass;
    UPROPERTY()
    UWUIManager* UIManager;
private:
    // Переменные состояния игры
    bool bGameEnded;

};