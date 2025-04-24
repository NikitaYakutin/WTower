#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "WTowerSaveGame.h"

#include "WTowerGameInstance.generated.h"


/**
 * Game Instance for managing save data and settings across level transitions
 */
UCLASS()
class WTOWER_API UWTowerGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UWTowerGameInstance();
    // Добавьте в вашем WTowerGameInstance.h:



    virtual void Init() override;

    // Save the current game data
    UFUNCTION(BlueprintCallable, Category = "Save Game")
    void SaveGameData();

    // Load the saved game data
    UFUNCTION(BlueprintCallable, Category = "Save Game")
    bool LoadGameData();

    // Update best score if current score is better
    UFUNCTION(BlueprintCallable, Category = "Save Game")
    bool UpdateBestScore(int32 CurrentScore);

    // Update best completion time if current time is better
    UFUNCTION(BlueprintCallable, Category = "Save Game")
    bool UpdateBestCompletionTime(float CurrentTime);

    // Get best score
    UFUNCTION(BlueprintPure, Category = "Save Game")
    int32 GetBestScore() const;

    // Get best completion time
    UFUNCTION(BlueprintPure, Category = "Save Game")
    float GetBestCompletionTime() const;

    // Settings getters
    UFUNCTION(BlueprintPure, Category = "Settings")
    float GetMasterVolume() const;

    UFUNCTION(BlueprintPure, Category = "Settings")
    float GetMusicVolume() const;

    UFUNCTION(BlueprintPure, Category = "Settings")
    float GetSFXVolume() const;

    UFUNCTION(BlueprintPure, Category = "Settings")
    FIntPoint GetWindowSize() const;

    UFUNCTION(BlueprintPure, Category = "Settings")
    bool IsFullscreen() const;

    // Settings setters
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void SetMasterVolume(float Volume);

    UFUNCTION(BlueprintCallable, Category = "Settings")
    void SetMusicVolume(float Volume);

    UFUNCTION(BlueprintCallable, Category = "Settings")
    void SetSFXVolume(float Volume);

    UFUNCTION(BlueprintCallable, Category = "Settings")
    void SetWindowSize(FIntPoint Size);

    UFUNCTION(BlueprintCallable, Category = "Settings")
    void SetFullscreen(bool bFullscreen);

    // Apply settings to game
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void ApplyAudioSettings();

    UFUNCTION(BlueprintCallable, Category = "Settings")
    void ApplyWindowSettings();

private:
    // Reference to the save game object
    UPROPERTY()
    UWTowerSaveGame* CurrentSaveGame;

    // Save slot name
    FString SaveSlotName;
    // В WTowerGameInstance.h добавить:

};