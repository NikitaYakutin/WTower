#pragma once

#include "CoreMinimal.h"
#include "MenuWidget/WSettingsMenuWidget.h"
#include "MenuWidget/WPauseMenuWidget.h"
#include "MenuWidget/WVictoryScreenWidget.h"
#include "MenuWidget/WDefeatMenuWidget.h"
#include <WTower/WTowerHUDWidget.h>
#include "GameFramework/GameModeBase.h"
#include "MenuWidget/WMainMenuWidget.h"
#include "MenuGameMode.generated.h"




class UWUIManager;

/**
 * GameMode for managing all menus
 */
UCLASS()
class WTOWER_API AMenuGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMenuGameMode();

    // Game initialization
    virtual void BeginPlay() override;

    // Menu management methods
    UFUNCTION(BlueprintCallable, Category = "UI")
    void CloseCurrentMenu();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void ReturnToPreviousMenu();

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UWMainMenuWidget> MainMenuWidgetClass;
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UWSettingsMenuWidget> SettingsMenuWidgetClass;
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UWPauseMenuWidget> PauseMenuWidgetClass;
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UWVictoryScreenWidget> VictoryScreenWidgetClass;
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UWDefeatMenuWidget> DefeatMenuWidgetClass;
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UWTowerHUDWidget> HUDWidgetClass;
    
private:
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UWUIManager> UIManagerClass;
    
    // UI Manager
    UPROPERTY()
    UWUIManager* UIManager;
};