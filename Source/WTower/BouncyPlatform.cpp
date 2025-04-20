// BouncyPlatform.cpp
#include "BouncyPlatform.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Audio/WAudioManagerActor.h"
ABouncyPlatform::ABouncyPlatform()
{
    // Устанавливаем тип по умолчанию как Bouncy
    PlatformType = EPlatformType::Bouncy;

    // Настройки по умолчанию
    bScaleOnBounce = true;
    BounceVisualStrength = 0.5f; // Насколько сильное сжатие будет при отскоке
    bPlayEffectsOnBounce = true;

    // Устанавливаем множитель отскока
    BounceMultiplier = 1.5f;

    // Минимальное время между активациями
    CooldownTime = 0.1f;
}

void ABouncyPlatform::BeginPlay()
{
    Super::BeginPlay();

    // Убеждаемся, что платформа установлена как тип Bouncy
    PlatformType = EPlatformType::Bouncy;

    // Инициализируем время последней активации
    LastActivationTime = 0.0f;

    // Сохраняем оригинальный масштаб платформы
    if (PlatformMesh)
    {
        OriginalPlatformScale = PlatformMesh->GetRelativeScale3D();
    }
}

void ABouncyPlatform::ActivatePlatform(AActor* Activator)
{
    // Проверка на кулдаун активации
    if (GetWorld()->GetTimeSeconds() - LastActivationTime < CooldownTime)
    {
        return; // Слишком рано для повторной активации
    }

    // Обновляем время последней активации
    LastActivationTime = GetWorld()->GetTimeSeconds();

    AWAudioManagerActor* AudioManager = Cast<AWAudioManagerActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AWAudioManagerActor::StaticClass()));
    if (AudioManager)
    {
        AudioManager->PlaySoundAtLocation(BouncySound, GetActorLocation());
    }

    // Применяем отскок к персонажу
    BouncePlatform(Activator);

    // Вызываем базовую реализацию
    Super::ActivatePlatform(Activator);
}

void ABouncyPlatform::BouncePlatform(AActor* Activator)
{
    // Визуальный эффект сжатия и растяжения
    if (PlatformMesh && bScaleOnBounce)
    {
        // Отменить предыдущий таймер, если он ещё активен
        if (GetWorldTimerManager().IsTimerActive(ScaleTimerHandle))
        {
            GetWorldTimerManager().ClearTimer(ScaleTimerHandle);
        }

        // Убедимся, что у нас есть оригинальный масштаб
        if (!OriginalPlatformScale.IsSet())
        {
            OriginalPlatformScale = PlatformMesh->GetRelativeScale3D();
        }

        // Используем оригинальный масштаб вместо текущего
        FVector OriginalScale = OriginalPlatformScale.GetValue();
        float CompressRatio = 1.0f - BounceVisualStrength;
        FVector CompressedScale = FVector(
            OriginalScale.X * (1.0f + BounceVisualStrength * 0.2f),
            OriginalScale.Y * (1.0f + BounceVisualStrength * 0.2f),
            OriginalScale.Z * CompressRatio);
        PlatformMesh->SetRelativeScale3D(CompressedScale);

        // Возвращаем к исходному размеру с задержкой
        GetWorld()->GetTimerManager().SetTimer(ScaleTimerHandle, [this]()
            {
                if (PlatformMesh && OriginalPlatformScale.IsSet())
                {
                    PlatformMesh->SetRelativeScale3D(OriginalPlatformScale.GetValue());
                }
            }, 0.15f, false);
    }

    // Визуальные эффекты если нужны
    if (bPlayEffectsOnBounce)
    {
        // Здесь можно добавить частицы, звуки и т.д.
        // Для примера:
        /*
        if (BounceEffect)
        {
            UGameplayStatics::SpawnEmitterAtLocation(
                GetWorld(),
                BounceEffect,
                GetActorLocation() + FVector(0, 0, 10),
                FRotator::ZeroRotator
            );
        }
        */
    }

    // Применяем дополнительный отскок к персонажу
    APlayerCharacter* Player = Cast<APlayerCharacter>(Activator);
    if (Player && Player->GetCharacterMovement())
    {
        UE_LOG(LogTemp, Display, TEXT("Bouncy platform: Applying bounce factor %f"), BounceMultiplier);

        // Сохраняем оригинальную скорость прыжка
        float OriginalJumpVelocity = Player->GetCharacterMovement()->JumpZVelocity;

        // Вычисляем скорость отскока
        float BounceVelocity = OriginalJumpVelocity * BounceMultiplier;

        // Используем LaunchCharacter вместо прямой манипуляции с Velocity
        // и вызова Jump() - это более надёжный метод для отскока
        Player->LaunchCharacter(
            FVector(
                Player->GetCharacterMovement()->Velocity.X * 0.8f, // сохраняем часть горизонтальной скорости
                Player->GetCharacterMovement()->Velocity.Y * 0.8f,
                BounceVelocity
            ),
            false,  // не перезаписывать X компоненту
            true    // перезаписывать Z компоненту
        );

        // Опционально: сбрасываем состояние прыжка чтобы игрок мог снова прыгать
        Player->JumpCurrentCount = 0;


    }

    // Не вызываем базовую реализацию, чтобы не дублировать функциональность
}