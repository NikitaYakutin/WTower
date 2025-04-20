#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
#include "WAudioManagerActor.generated.h"

UCLASS(Blueprintable, BlueprintType)
class WTOWER_API AWAudioManagerActor : public AActor
{
    GENERATED_BODY()

public:
    AWAudioManagerActor();

protected:
    virtual void BeginPlay() override;

public:
    // Компоненты для стриминга музыки
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UAudioComponent* MusicComponent;
    // Update audio settings from GameInstance
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void UpdateSettingsFromGameInstance();
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void UpdateVolumes();
    // Звуки, доступные из Blueprint
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds|Music")
    USoundBase* MenuMusic;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds|Music")
    USoundBase* GameplayMusic;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds|Effects")
    USoundBase* ButtonClickSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds|Character")
    USoundBase* JumpSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds|Character")
    USoundBase* LandSound;

    // Настройки громкости
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    float MasterVolume;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    float MusicVolume;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    float SFXVolume;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    bool bMuteAudio;

    // Функции управления звуком
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlayMusic(USoundBase* Music);

    UFUNCTION(BlueprintCallable, Category = "Audio")
    void StopMusic();

    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlayMenuMusic();

    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlayGameplayMusic();

    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlaySound2D(USoundBase* Sound);

    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlaySoundAtLocation(USoundBase* Sound, FVector Location);



private:



};