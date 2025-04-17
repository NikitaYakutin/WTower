// VictoryItemPowerUp.cpp
#include "VictoryItemPowerUp.h"
#include "PlayerCharacter.h"
#include "WTowerGameState.h"
#include "Kismet/GameplayStatics.h"

AVictoryItemPowerUp::AVictoryItemPowerUp()
{
    PowerUpType = EPowerUpType::Victory;
    Duration = 0.0f; // Нет длительности, эффект мгновенный
    VictoryBonusScore = 1000;
}

void AVictoryItemPowerUp::ApplyPowerUpEffect_Implementation(AActor* Target)
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

    // Добавляем бонусные очки за победу
    GameState->AddScore(VictoryBonusScore);

    // Воспроизводим звук победы, если задан
    if (VictorySound)
    {
        UGameplayStatics::PlaySound2D(Character->GetWorld(), VictorySound);
    }

    // Создаем эффект победы, если он определен
    if (VictoryEffect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(
            Character->GetWorld(),
            VictoryEffect,
            Character->GetActorLocation(),
            FRotator::ZeroRotator,
            true
        );
    }

    // Уведомляем игрока об активации усиления
    Character->NotifyPowerUpActivated(PowerUpType, 0.0f);

    // Отмечаем игру как завершенную
    GameState->SetGameCompleted();
    Character->CompleteGame();
}