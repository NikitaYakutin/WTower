#include "WAudioManagerActor.h"
#include "Kismet/GameplayStatics.h"
#include "../WTowerGameInstance.h"
#include "../Config/WTowerGameConfig.h"

AWAudioManagerActor::AWAudioManagerActor()
{
    PrimaryActorTick.bCanEverTick = false;

    // Создаем компонент для музыки
    MusicComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("MusicComponent"));
    MusicComponent->bAutoActivate = false;
    RootComponent = MusicComponent;

    // Инициализация значений громкости
    MasterVolume = 1.0f;
    MusicVolume = 0.7f;
    SFXVolume = 1.0f;
    bMuteAudio = false;
}

void AWAudioManagerActor::BeginPlay()
{
    Super::BeginPlay();

    // Получаем GameInstance при старте
    GameInstance = Cast<UWTowerGameInstance>(GetGameInstance());

    // Загружаем настройки из конфига
    LoadSettingsFromConfig();

    // Делаем звуковой компонент не разрушаемым при смене уровней
    MusicComponent->bStopWhenOwnerDestroyed = false;

    // Обеспечиваем, чтобы аудио менеджер не уничтожался при смене уровней
    SetActorTickEnabled(false);
}

void AWAudioManagerActor::PlayMusic(USoundBase* Music)
{
    if (Music && MusicComponent)
    {
        // Останавливаем текущую музыку
        MusicComponent->Stop();

        // Устанавливаем новую музыку
        MusicComponent->SetSound(Music);

        // Настраиваем громкость
        float FinalVolume = bMuteAudio ? 0.0f : (MasterVolume * MusicVolume);
        MusicComponent->SetVolumeMultiplier(FinalVolume);

        // Запускаем воспроизведение
        MusicComponent->Play();
    }
}

void AWAudioManagerActor::StopMusic()
{
    if (MusicComponent)
    {
        MusicComponent->Stop();
    }
}

void AWAudioManagerActor::PlayMenuMusic()
{
    PlayMusic(MenuMusic);
}

void AWAudioManagerActor::PlayGameplayMusic()
{
    PlayMusic(GameplayMusic);
}

void AWAudioManagerActor::PlaySound2D(USoundBase* Sound)
{
    if (Sound && !bMuteAudio)
    {
        float FinalVolume = MasterVolume * SFXVolume;
        UGameplayStatics::PlaySound2D(this, Sound, FinalVolume);
    }
}

void AWAudioManagerActor::PlaySoundAtLocation(USoundBase* Sound, FVector Location)
{
    if (Sound && !bMuteAudio)
    {
        float FinalVolume = MasterVolume * SFXVolume;
        UGameplayStatics::PlaySoundAtLocation(this, Sound, Location, FRotator::ZeroRotator, FinalVolume);
    }
}

void AWAudioManagerActor::SetMasterVolume(float Volume)
{
    MasterVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
    UpdateVolumes();
    SaveSettingsToConfig();
}

void AWAudioManagerActor::SetMusicVolume(float Volume)
{
    MusicVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
    UpdateVolumes();
    SaveSettingsToConfig();
}

void AWAudioManagerActor::SetSFXVolume(float Volume)
{
    SFXVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
    SaveSettingsToConfig();
}

void AWAudioManagerActor::MuteAudio(bool bMute)
{
    bMuteAudio = bMute;
    UpdateVolumes();
    SaveSettingsToConfig();
}

void AWAudioManagerActor::UpdateVolumes()
{
    if (MusicComponent)
    {
        float FinalVolume = bMuteAudio ? 0.0f : (MasterVolume * MusicVolume);
        MusicComponent->SetVolumeMultiplier(FinalVolume);
    }
}

void AWAudioManagerActor::LoadSettingsFromConfig()
{
    if (GameInstance && GameInstance->GetGameConfig())
    {
        UWTowerGameConfig* Config = GameInstance->GetGameConfig();

        // Загружаем значения громкости
        MasterVolume = Config->MasterVolume;
        MusicVolume = Config->MusicVolume;
        SFXVolume = Config->SFXVolume;
        bMuteAudio = Config->bMuteAudio;

        // Применяем загруженные настройки
        UpdateVolumes();
    }
}

void AWAudioManagerActor::SaveSettingsToConfig()
{
    if (GameInstance && GameInstance->GetGameConfig())
    {
        UWTowerGameConfig* Config = GameInstance->GetGameConfig();

        // Сохраняем настройки в конфиг
        Config->MasterVolume = MasterVolume;
        Config->MusicVolume = MusicVolume;
        Config->SFXVolume = SFXVolume;
        Config->bMuteAudio = bMuteAudio;

        // Сохраняем конфиг в файл
        GameInstance->SaveGameConfig();
    }
}