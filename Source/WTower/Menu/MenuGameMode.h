#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MenuGameMode.generated.h"

class UWUIManager;

/**
 * GameMode для управления всеми меню
 */
UCLASS()
class WTOWER_API AMenuGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMenuGameMode();

    // Инициализация игры
    virtual void BeginPlay() override;

    // Методы для работы с меню
    UFUNCTION(BlueprintCallable, Category = "UI")
    void CloseCurrentMenu();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void ReturnToPreviousMenu();

    // Получить UIManager
    UFUNCTION(BlueprintPure, Category = "UI")
    UWUIManager* GetUIManager() const { return UIManager; }
    // Классы виджетов для UI
    UPROPERTY(EditDefaultsOnly, Category = "UI|Widgets")
    TSubclassOf<class UWMainMenuWidget> MainMenuWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI|Widgets")
    TSubclassOf<class UWPauseMenuWidget> PauseMenuWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI|Widgets")
    TSubclassOf<class UWSettingsMenuWidget> SettingsMenuWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI|Widgets")
    TSubclassOf<class UWVictoryScreenWidget> VictoryScreenWidgetClass;
private:
    // В вашем MenuGameMode.h добавьте:
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UWUIManager> UIManagerClass;
    // Менеджер пользовательского интерфейса
    UPROPERTY()
    UWUIManager* UIManager;
};