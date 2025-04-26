// SpeedBoostPowerUp.cpp
#include "SpeedBoostPowerUp.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Audio/WAudioManagerActor.h"

ASpeedBoostPowerUp::ASpeedBoostPowerUp()
{
    PowerUpType = EPowerUpType::SpeedBoost;
    Duration = 5.0f;
    SpeedMultiplier = 1.5f;
    Strength = SpeedMultiplier;
}

void ASpeedBoostPowerUp::ApplyPowerUpEffect_Implementation(AActor* Target)
{
    APlayerCharacter* Character = Cast<APlayerCharacter>(Target);
    if (!Character)
    {
        return;
    }

    // Получаем компонент движения
    UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement();
    if (!MovementComp)
    {
        return;
    }

    // Сохраняем исходное значение скорости
    float OriginalSpeed = MovementComp->MaxWalkSpeed;

    // Применяем новое значение
    MovementComp->MaxWalkSpeed *= SpeedMultiplier;

    // Создаем эффект скорости, если он определен
    UParticleSystemComponent* SpeedEffect = nullptr;
    if (SpeedEffectTemplate)
    {
        SpeedEffect = UGameplayStatics::SpawnEmitterAttached(
            SpeedEffectTemplate,
            Character->GetMesh(),
            NAME_None,
            FVector::ZeroVector,
            FRotator::ZeroRotator,
            EAttachLocation::SnapToTarget,
            true
        );
    }

    // Сохраняем позицию для использования в лямбде
    FVector EffectLocation = GetActorLocation();
    USoundBase* SoundToPlay = ExpireSound; // Сохраняем звук для использования в лямбде

    // Получаем ссылку на аудио менеджер
    AWAudioManagerActor* AudioManager = nullptr;
    TArray<AActor*> AudioManagers;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWAudioManagerActor::StaticClass(), AudioManagers);
    if (AudioManagers.Num() > 0)
    {
        AudioManager = Cast<AWAudioManagerActor>(AudioManagers[0]);
    }

    // Через определенное время восстанавливаем исходное значение
    FTimerHandle TimerHandle;
    FTimerDelegate TimerDelegate;

    TimerDelegate.BindLambda([MovementComp, OriginalSpeed, SpeedEffect, Character, SoundToPlay, EffectLocation, AudioManager]() {
        // Восстанавливаем исходное значение
        MovementComp->MaxWalkSpeed = OriginalSpeed;

        // Удаляем эффект
        if (SpeedEffect)
        {
            SpeedEffect->DestroyComponent();
        }

        // Воспроизводим звук окончания эффекта через аудио менеджер
        if (SoundToPlay && Character && Character->IsValidLowLevel())
        {
            if (AudioManager)
            {
                AudioManager->PlaySoundAtLocation(SoundToPlay, EffectLocation);
            }
            else
            {
                // Запасной вариант, если аудио менеджер не доступен
                UGameplayStatics::PlaySoundAtLocation(
                    Character,
                    SoundToPlay,
                    EffectLocation,
                    1.0f,
                    1.0f
                );
            }
        }
        });

    GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, Duration, false);

    // Уведомляем игрока об активации усиления
    Character->NotifyPowerUpActivated(PowerUpType, Duration);
}