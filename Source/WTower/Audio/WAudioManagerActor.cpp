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

// Упрощенная реализация PlaySound2D для музыки меню
void AWAudioManagerActor::PlaySound2D(USoundBase* Sound)
{
    if (!Sound)
    {
        return;
    }

    // Рассчитываем громкость в зависимости от настроек
    float FinalVolume = bMuteAudio ? 0.0f : (MasterVolume * MusicVolume);

    // Если уже есть активная музыка меню, останавливаем её
    if (MenuMusicComponent && MenuMusicComponent->IsValidLowLevel())
    {
        MenuMusicComponent->Stop();
    }

    // Создаем новый аудио компонент для музыки меню
    MenuMusicComponent = UGameplayStatics::CreateSound2D(this, Sound, FinalVolume);

    if (MenuMusicComponent)
    {
        MenuMusicComponent->Play();
        UE_LOG(LogTemp, Log, TEXT("Playing menu music: %s with volume: %f"), *Sound->GetName(), FinalVolume);
    }
}

// Метод для остановки музыки меню
void AWAudioManagerActor::StopMenuMusic()
{
    if (MenuMusicComponent && MenuMusicComponent->IsValidLowLevel() && MenuMusicComponent->IsPlaying())
    {
        MenuMusicComponent->Stop();
        UE_LOG(LogTemp, Log, TEXT("Stopped menu music"));
    }

    // Очищаем указатель
    MenuMusicComponent = nullptr;
}

// Метод для проверки, играет ли музыка меню
bool AWAudioManagerActor::IsMenuMusicPlaying() const
{
    return MenuMusicComponent && MenuMusicComponent->IsValidLowLevel() && MenuMusicComponent->IsPlaying();
}


void AWAudioManagerActor::PlaySoundAtLocation(USoundBase* Sound, FVector Location)
{
    if (Sound && !bMuteAudio)
    {
        float FinalVolume = MasterVolume * SFXVolume;
        UGameplayStatics::PlaySoundAtLocation(this, Sound, Location, FRotator::ZeroRotator, FinalVolume);
    }
}


// Обновите метод UpdateVolumes, чтобы музыка меню также обновлялась
void AWAudioManagerActor::UpdateVolumes()
{
    if (MusicComponent)
    {
        float FinalVolume = bMuteAudio ? 0.0f : (MasterVolume * MusicVolume);
        MusicComponent->SetVolumeMultiplier(FinalVolume);
    }

    // Обновляем громкость музыки меню, если она играет
    if (MenuMusicComponent && MenuMusicComponent->IsValidLowLevel() && MenuMusicComponent->IsPlaying())
    {
        float FinalVolume = bMuteAudio ? 0.0f : (MasterVolume * MusicVolume);
        MenuMusicComponent->SetVolumeMultiplier(FinalVolume);
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