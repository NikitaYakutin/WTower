// BouncyPlatform.cpp
#include "BouncyPlatform.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

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
}

void ABouncyPlatform::BeginPlay()
{
    Super::BeginPlay();
    
    // Убеждаемся, что платформа установлена как тип Bouncy
    PlatformType = EPlatformType::Bouncy;
}

void ABouncyPlatform::ActivatePlatform(AActor* Activator)
{
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
        // Сжимаем платформу
        FVector OriginalScale = PlatformMesh->GetRelativeScale3D();
        float CompressRatio = 1.0f - BounceVisualStrength;
        FVector CompressedScale = FVector(
            OriginalScale.X * (1.0f + BounceVisualStrength * 0.2f), 
            OriginalScale.Y * (1.0f + BounceVisualStrength * 0.2f), 
            OriginalScale.Z * CompressRatio);
        PlatformMesh->SetRelativeScale3D(CompressedScale);

        // Возвращаем к исходному размеру с задержкой
        FTimerHandle ScaleTimerHandle;
        GetWorld()->GetTimerManager().SetTimer(ScaleTimerHandle, [this, OriginalScale]()
            {
                if (PlatformMesh)
                {
                    PlatformMesh->SetRelativeScale3D(OriginalScale);
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

        // Сохраняем текущую скорость прыжка и увеличиваем её
        float OriginalJumpVelocity = Player->GetCharacterMovement()->JumpZVelocity;
        Player->GetCharacterMovement()->JumpZVelocity = OriginalJumpVelocity * BounceMultiplier;

        // Применяем импульс напрямую для более надежного эффекта
        Player->GetCharacterMovement()->Velocity = FVector(
            Player->GetCharacterMovement()->Velocity.X,
            Player->GetCharacterMovement()->Velocity.Y,
            Player->GetCharacterMovement()->JumpZVelocity
        );
        Player->Jump();

        // Возвращаем исходную скорость прыжка
        FTimerHandle ResetTimerHandle;
        GetWorld()->GetTimerManager().SetTimer(ResetTimerHandle, [Player, OriginalJumpVelocity]()
            {
                if (Player && Player->GetCharacterMovement())
                {
                    Player->GetCharacterMovement()->JumpZVelocity = OriginalJumpVelocity;
                }
            }, 0.5f, false);
    }
    
    // Не вызываем базовую реализацию, чтобы не дублировать функциональность
}