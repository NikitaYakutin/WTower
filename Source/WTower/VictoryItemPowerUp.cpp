// VictoryItemPowerUp.cpp
#include "VictoryItemPowerUp.h"
#include "PlayerCharacter.h"
#include "WTowerGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Audio/WAudioManagerActor.h"

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
        AWAudioManagerActor* AudioManager = Cast<AWAudioManagerActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AWAudioManagerActor::StaticClass()));
        if (AudioManager)
        {


            AudioManager->PlaySoundAtLocation(VictorySound,GetActorLocation());

        }
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