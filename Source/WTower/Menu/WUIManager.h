#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WUIManager.generated.h"

// Menu types enum
UENUM(BlueprintType)
enum class EWMenuType : uint8
{
    None,
    Main,
    Pause,
    Settings,
    Victory,
    Defeat,
    HUD
};

/**
 * Manager class for handling all UI menus and their transitions
 */
UCLASS(Blueprintable, BlueprintType)
class WTOWER_API UWUIManager : public UObject
{
    GENERATED_BODY()

public:
    UWUIManager();
    
    // Initialize with player controller
    void Initialize(APlayerController* InController);
    
    // Set widget classes
    void SetWidgetClasses(
        TSubclassOf<class UWMainMenuWidget> InMainMenuWidgetClass,
        TSubclassOf<class UWPauseMenuWidget> InPauseMenuWidgetClass,
        TSubclassOf<class UWSettingsMenuWidget> InSettingsMenuWidgetClass,
        TSubclassOf<class UWVictoryScreenWidget> InVictoryScreenWidgetClass,
        TSubclassOf<class UWDefeatMenuWidget> InDefeatMenuWidgetClass = nullptr,
        TSubclassOf<class UWTowerHUDWidget> InHUDWidgetClass = nullptr
    );
    
    // Menu navigation methods
    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowMenu(EWMenuType MenuType, const FString& Param = "");
    
    UFUNCTION(BlueprintCallable, Category = "UI")
    void CloseCurrentMenu();
    
    UFUNCTION(BlueprintCallable, Category = "UI")
    void ReturnToPreviousMenu();
    
    // Special menu functions
    UFUNCTION(BlueprintCallable, Category = "UI")
    void TogglePauseMenu();
    
    UFUNCTION(BlueprintCallable, Category = "UI")
    void OpenSettings();
    
    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowVictoryScreen(int32 Score, float CompletionTime);
    
    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowDefeatScreen(const FString& Reason);
    
    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowHUD();
    
    UFUNCTION(BlueprintCallable, Category = "UI")
    void HideHUD();
    
    // Get current menu type
    UFUNCTION(BlueprintPure, Category = "UI")
    EWMenuType GetCurrentMenuType() const { return CurrentMenuType; }
    
    // Check if we're in any menu
    UFUNCTION(BlueprintPure, Category = "UI")
    bool IsInMenu() const { return CurrentMenuType != EWMenuType::None && CurrentMenuType != EWMenuType::HUD; }
    
    // Set pause state
    UFUNCTION(BlueprintCallable, Category = "UI")
    void SetGamePaused(bool bPaused);

private:
    // Create widget of specified class if needed
    template<typename T>
    T* CreateWidgetIfNeeded(TSubclassOf<T>& WidgetClass, T*& WidgetRef);
    
    // Template helper to clean up a widget properly
    template<typename T>
    void CleanupWidget(T*& Widget);

private:
    // Player controller reference
    UPROPERTY()
    APlayerController* PlayerController;
    
    // Widget class references
    UPROPERTY()
    TSubclassOf<class UWMainMenuWidget> MainMenuWidgetClass;
    
    UPROPERTY()
    TSubclassOf<class UWPauseMenuWidget> PauseMenuWidgetClass;
    
    UPROPERTY()
    TSubclassOf<class UWSettingsMenuWidget> SettingsMenuWidgetClass;
    
    UPROPERTY()
    TSubclassOf<class UWVictoryScreenWidget> VictoryScreenWidgetClass;
    
    UPROPERTY()
    TSubclassOf<class UWDefeatMenuWidget> DefeatMenuWidgetClass;
    
    UPROPERTY()
    TSubclassOf<class UWTowerHUDWidget> HUDWidgetClass;
    
    // Widget instances
    UPROPERTY()
    class UWMainMenuWidget* MainMenuWidget;
    
    UPROPERTY()
    class UWPauseMenuWidget* PauseMenuWidget;
    
    UPROPERTY()
    class UWSettingsMenuWidget* SettingsMenuWidget;
    
    UPROPERTY()
    class UWVictoryScreenWidget* VictoryScreenWidget;
    
    UPROPERTY()
    class UWDefeatMenuWidget* DefeatMenuWidget;
    
    UPROPERTY()
    class UWTowerHUDWidget* HUDWidget;
    
    // Current active menu
    EWMenuType CurrentMenuType;
    
    // Menu history for navigation
    TArray<EWMenuType> MenuHistory;
    
    // Flag to track if in-game
    bool bIsInGameplay;
};