#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "WTowerSaveGame.generated.h"

UCLASS()
class WTOWER_API UWTowerSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UWTowerSaveGame();

    // Best score achieved
    UPROPERTY(VisibleAnywhere, Category = "SaveData")
    int32 BestScore;

    // Best completion time (lower is better)
    UPROPERTY(VisibleAnywhere, Category = "SaveData")
    float BestCompletionTime;

    // Settings
    UPROPERTY(VisibleAnywhere, Category = "Settings")
    float MasterVolume;

    UPROPERTY(VisibleAnywhere, Category = "Settings")
    float MusicVolume;

    UPROPERTY(VisibleAnywhere, Category = "Settings")
    float SFXVolume;

    // Window settings
    UPROPERTY(VisibleAnywhere, Category = "Settings")
    FIntPoint WindowSize;

    UPROPERTY(VisibleAnywhere, Category = "Settings")
    bool bIsFullscreen;

    // Save slot name
    UPROPERTY(VisibleAnywhere, Category = "SaveData")
    FString SaveSlotName;
};