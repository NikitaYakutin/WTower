// JumpBoostPowerUp.cpp
#include "JumpBoostPowerUp.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AJumpBoostPowerUp::AJumpBoostPowerUp()
{
    PowerUpType = EPowerUpType::ExtraJump;
    Duration = 4.0f;
    JumpHeightMultiplier = 1.3f;
    Strength = JumpHeightMultiplier;
}

void AJumpBoostPowerUp::ApplyPowerUpEffect_Implementation(AActor* Target)
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

    // Сохраняем исходное значение высоты прыжка
    float OriginalJumpZ = MovementComp->JumpZVelocity;

    // Применяем новое значение
    MovementComp->JumpZVelocity *= JumpHeightMultiplier;

    // Создаем эффект прыжка, если он определен
    UParticleSystemComponent* JumpEffect = nullptr;
    if (JumpEffectTemplate)
    {
        JumpEffect = UGameplayStatics::SpawnEmitterAttached(
            JumpEffectTemplate,
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

    TimerDelegate.BindLambda([MovementComp, OriginalJumpZ, JumpEffect, Character, SoundToPlay, EffectLocation]() {
        // Восстанавливаем исходное значение
        MovementComp->JumpZVelocity = OriginalJumpZ;

        // Удаляем эффект
        if (JumpEffect)
        {
            JumpEffect->DestroyComponent();
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