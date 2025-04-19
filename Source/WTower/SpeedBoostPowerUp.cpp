// SpeedBoostPowerUp.cpp
#include "SpeedBoostPowerUp.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

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

    // Сохраняем позицию и звук для использования в лямбде
    FVector EffectLocation = GetActorLocation();
    USoundBase* SoundToPlay = ExpireSound; // Новая строка

    // Через определенное время восстанавливаем исходное значение
    FTimerHandle TimerHandle;
    FTimerDelegate TimerDelegate;

    TimerDelegate.BindLambda([MovementComp, OriginalSpeed, SpeedEffect, Character, SoundToPlay, EffectLocation]() {
        // Восстанавливаем исходное значение
        MovementComp->MaxWalkSpeed = OriginalSpeed;

        // Удаляем эффект
        if (SpeedEffect)
        {
            SpeedEffect->DestroyComponent();
        }

        // Воспроизводим звук окончания эффекта
        if (SoundToPlay && Character && Character->IsValidLowLevel())
        {
            UGameplayStatics::PlaySoundAtLocation(
                Character,    // WorldContextObject
                SoundToPlay,  // Звук
                EffectLocation, // Позиция
                1.0f,         // Громкость
                1.0f          // Высота звука
            );
        }
        });

    GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, Duration, false);

    // Уведомляем игрока об активации усиления
    Character->NotifyPowerUpActivated(PowerUpType, Duration);
}