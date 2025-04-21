#include "WTowerGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameUserSettings.h"
#include "Audio/WAudioManagerActor.h"

UWTowerGameInstance::UWTowerGameInstance()
{
    SaveSlotName = TEXT("WTowerSaveSlot");
    CurrentSaveGame = nullptr;
}

void UWTowerGameInstance::Init()
{
    Super::Init();
    
    // Load saved game data on startup
    LoadGameData();
    
    // Apply settings
    ApplyAudioSettings();
    ApplyWindowSettings();
}

void UWTowerGameInstance::SaveGameData()
{
    // If we don't have a save game, create one
    if (!CurrentSaveGame)
    {
        CurrentSaveGame = Cast<UWTowerSaveGame>(UGameplayStatics::CreateSaveGameObject(UWTowerSaveGame::StaticClass()));
    }
    
    if (CurrentSaveGame)
    {
        // Save to disk
        UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SaveSlotName, 0);
        UE_LOG(LogTemp, Log, TEXT("Game data saved successfully."));
    }
}

bool UWTowerGameInstance::LoadGameData()
{
    // Check if save game exists
    if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
    {
        // Load the save game
        CurrentSaveGame = Cast<UWTowerSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
        
        if (CurrentSaveGame)
        {
            UE_LOG(LogTemp, Log, TEXT("Game data loaded successfully. Best Score: %d, Best Time: %f"), 
                CurrentSaveGame->BestScore, CurrentSaveGame->BestCompletionTime);
            return true;
        }
    }
    
    // If no save game exists or loading failed, create a new one
    CurrentSaveGame = Cast<UWTowerSaveGame>(UGameplayStatics::CreateSaveGameObject(UWTowerSaveGame::StaticClass()));
    
    if (CurrentSaveGame)
    {
        UE_LOG(LogTemp, Log, TEXT("New save game created."));
        return true;
    }
    
    UE_LOG(LogTemp, Error, TEXT("Failed to create or load save game!"));
    return false;
}

bool UWTowerGameInstance::UpdateBestScore(int32 CurrentScore)
{
    if (!CurrentSaveGame)
    {
        LoadGameData();
    }
    
    if (CurrentScore > CurrentSaveGame->BestScore)
    {
        CurrentSaveGame->BestScore = CurrentScore;
        SaveGameData();
        return true;
    }
    
    return false;
}

bool UWTowerGameInstance::UpdateBestCompletionTime(float CurrentTime)
{
    if (!CurrentSaveGame)
    {
        LoadGameData();
    }
    
    // Lower time is better
    if (CurrentTime > 0.0f && CurrentTime < CurrentSaveGame->BestCompletionTime)
    {
        CurrentSaveGame->BestCompletionTime = CurrentTime;
        SaveGameData();
        return true;
    }
    
    return false;
}

int32 UWTowerGameInstance::GetBestScore() const
{
    return CurrentSaveGame ? CurrentSaveGame->BestScore : 0;
}

float UWTowerGameInstance::GetBestCompletionTime() const
{
    return CurrentSaveGame ? CurrentSaveGame->BestCompletionTime : 999999.0f;
}

float UWTowerGameInstance::GetMasterVolume() const
{
    return CurrentSaveGame ? CurrentSaveGame->MasterVolume : 1.0f;
}

float UWTowerGameInstance::GetMusicVolume() const
{
    return CurrentSaveGame ? CurrentSaveGame->MusicVolume : 0.7f;
}

float UWTowerGameInstance::GetSFXVolume() const
{
    return CurrentSaveGame ? CurrentSaveGame->SFXVolume : 1.0f;
}

FIntPoint UWTowerGameInstance::GetWindowSize() const
{
    return CurrentSaveGame ? CurrentSaveGame->WindowSize : FIntPoint(1280, 720);
}

bool UWTowerGameInstance::IsFullscreen() const
{
    return CurrentSaveGame ? CurrentSaveGame->bIsFullscreen : false;
}

void UWTowerGameInstance::SetMasterVolume(float Volume)
{
    if (!CurrentSaveGame)
    {
        LoadGameData();
    }
    
    CurrentSaveGame->MasterVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
    SaveGameData();
}

void UWTowerGameInstance::SetMusicVolume(float Volume)
{
    if (!CurrentSaveGame)
    {
        LoadGameData();
    }
    
    CurrentSaveGame->MusicVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
    SaveGameData();
}

void UWTowerGameInstance::SetSFXVolume(float Volume)
{
    if (!CurrentSaveGame)
    {
        LoadGameData();
    }
    
    CurrentSaveGame->SFXVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
    SaveGameData();
}

void UWTowerGameInstance::SetWindowSize(FIntPoint Size)
{
    if (!CurrentSaveGame)
    {
        LoadGameData();
    }
    
    CurrentSaveGame->WindowSize = Size;
    SaveGameData();
}

void UWTowerGameInstance::SetFullscreen(bool bFullscreen)
{
    if (!CurrentSaveGame)
    {
        LoadGameData();
    }
    
    CurrentSaveGame->bIsFullscreen = bFullscreen;
    SaveGameData();
}

void UWTowerGameInstance::ApplyAudioSettings()
{
    if (!CurrentSaveGame)
    {
        LoadGameData();
    }
    
    // Find audio manager and apply settings
    TArray<AActor*> AudioManagers;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWAudioManagerActor::StaticClass(), AudioManagers);
    
    for (AActor* Actor : AudioManagers)
    {
        AWAudioManagerActor* AudioManager = Cast<AWAudioManagerActor>(Actor);
        if (AudioManager)
        {
            AudioManager->MasterVolume = CurrentSaveGame->MasterVolume;
            AudioManager->MusicVolume = CurrentSaveGame->MusicVolume;
            AudioManager->SFXVolume = CurrentSaveGame->SFXVolume;
            
            // Call update volumes on audio manager
            AudioManager->UpdateVolumes();
            break;
        }
    }
}

void UWTowerGameInstance::ApplyWindowSettings()
{
    if (!CurrentSaveGame)
    {
        LoadGameData();
    }
    
    // Get game user settings
    UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
    if (Settings)
    {
        if (CurrentSaveGame->bIsFullscreen)
        {
            Settings->SetFullscreenMode(EWindowMode::Fullscreen);
        }
        else
        {
            Settings->SetFullscreenMode(EWindowMode::Windowed);
            Settings->SetScreenResolution(CurrentSaveGame->WindowSize);
        }
        
        // Apply settings
        Settings->ApplySettings(false);
    }
}