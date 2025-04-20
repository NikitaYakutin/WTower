#include "WTowerSaveGame.h"

UWTowerSaveGame::UWTowerSaveGame()
{
    // Default values
    BestScore = 0;
    BestCompletionTime = 999999.0f; // High value so any completion time is better
    
    // Default settings
    MasterVolume = 1.0f;
    MusicVolume = 0.7f;
    SFXVolume = 1.0f;
    
    WindowSize = FIntPoint(1280, 720); // Default window size
    bIsFullscreen = false;
    
    SaveSlotName = TEXT("WTowerSaveSlot");
}