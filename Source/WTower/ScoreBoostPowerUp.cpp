// ScoreBoostPowerUp.cpp
#include "ScoreBoostPowerUp.h"
#include "PlayerCharacter.h"
#include "WTowerGameState.h"
#include "Kismet/GameplayStatics.h"

AScoreBoostPowerUp::AScoreBoostPowerUp()
{
    PowerUpType = EPowerUpType::ScoreBonus;
    Duration = 0.0f; // Мгновенный эффект без длительности
    ScoreBonus = 100; // Базовое количество очков
    Strength = ScoreBonus;
}

void AScoreBoostPowerUp::ApplyPowerUpEffect_Implementation(AActor* Target)
{
    APlayerCharacter* Character = Cast<APlayerCharacter>(Target);
    if (!Character)
    {
        return;
    }

    // Получаем GameState
    AWTowerGameState* GameState = Cast<AWTowerGameState>(Character->GetWorld()->GetGameState());
    if (!GameState)
    {
        return;
    }

    // Добавляем очки при подборе
    GameState->AddScore(ScoreBonus);

    // Создаем эффект очков, если он определен
    if (ScoreEffectTemplate)
    {
        UGameplayStatics::SpawnEmitterAttached(
            ScoreEffectTemplate,
            Character->GetMesh(),
            NAME_None,
            FVector::ZeroVector,
            FRotator::ZeroRotator,
            EAttachLocation::SnapToTarget,
            true
        );
    }

    // Уведомляем игрока об активации усиления (мгновенный эффект)
    Character->NotifyPowerUpActivated(PowerUpType, 0.0f);
}