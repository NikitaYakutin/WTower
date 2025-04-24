#include "WAudioManagerActor.h"
#include "Kismet/GameplayStatics.h"
#include <WTower/WTowerGameInstance.h>




AWAudioManagerActor::AWAudioManagerActor()
{
    PrimaryActorTick.bCanEverTick = false;

    // Создаем компонент для музыки
    MusicComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("MusicComponent"));
    MusicComponent->bAutoActivate = false;
    RootComponent = MusicComponent;

    // Инициализация значений громкости
    MasterVolume = 1.0f;
    MusicVolume = 0.5f;
    SFXVolume = 1.0f;
    bMuteAudio = false;
}

void AWAudioManagerActor::BeginPlay()
{
    Super::BeginPlay();


    // Делаем звуковой компонент не разрушаемым при смене уровней
    MusicComponent->bStopWhenOwnerDestroyed = false;

    // Обеспечиваем, чтобы аудио менеджер не уничтожался при смене уровней
    SetActorTickEnabled(false);

    // Загружаем настройки из GameInstance
    UpdateSettingsFromGameInstance();
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


void AWAudioManagerActor::UpdateVolumes()
{
    if (MusicComponent)
    {
        float FinalVolume = bMuteAudio ? 0.0f : (MasterVolume * MusicVolume);
        MusicComponent->SetVolumeMultiplier(FinalVolume);
    }
}

void AWAudioManagerActor::UpdateSettingsFromGameInstance()
{
    UWTowerGameInstance* GameInstance = Cast<UWTowerGameInstance>(GetGameInstance());
    if (GameInstance)
    {
        MasterVolume = GameInstance->GetMasterVolume();
        MusicVolume = GameInstance->GetMusicVolume();
        SFXVolume = GameInstance->GetSFXVolume();

        UpdateVolumes();

        UE_LOG(LogTemp, Log, TEXT("Audio Manager updated settings from GameInstance: Master=%f, Music=%f, SFX=%f"),
            MasterVolume, MusicVolume, SFXVolume);
    }
}