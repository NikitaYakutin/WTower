#include "WTowerGameState.h"
#include "WTowerGameInstance.h"
//----------------------------------------------------------------------------------------
// КОНСТРУКТОР И МЕТОДЫ ЖИЗНЕННОГО ЦИКЛА
//----------------------------------------------------------------------------------------

AWTowerGameState::AWTowerGameState()
{
    // Включаем Tick для обновления таймера
    PrimaryActorTick.bCanEverTick = true;
    
    // Инициализируем переменные
    Score = 0;
    GameTime = 0.0f;
    CompletionTime = 0.0f;
    bGameCompleted = false;
    MaxPlayerHeight = 0.0f;
    CurrentPlayerHeight = 0.0f;
}

void AWTowerGameState::BeginPlay()
{
    Super::BeginPlay();
}

void AWTowerGameState::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // Обновляем время игры, только если игра не завершена
    if (!bGameCompleted)
    {
        GameTime += DeltaTime;
    }
}

//----------------------------------------------------------------------------------------
// МЕТОДЫ СИСТЕМЫ ОЧКОВ
//----------------------------------------------------------------------------------------

void AWTowerGameState::AddScore(int32 Points)
{
    // Добавляем очки к текущему счету
    Score += Points;
    
    // Вызываем делегат для уведомления о изменении счета
    OnScoreChanged.Broadcast(Score);
}

void AWTowerGameState::ResetScore()
{
    // Сбрасываем счет на ноль
    Score = 0;
    
    // Вызываем делегат для уведомления о изменении счета
    OnScoreChanged.Broadcast(Score);
}

//----------------------------------------------------------------------------------------
// МЕТОДЫ СИСТЕМЫ ТАЙМЕРА
//----------------------------------------------------------------------------------------

void AWTowerGameState::SetGameCompleted()
{
    if (!bGameCompleted)
    {
        // Устанавливаем флаг завершения и сохраняем время завершения
        bGameCompleted = true;
        CompletionTime = GameTime;
        
        // Вызываем делегат для уведомления о завершении игры
        OnGameCompleted.Broadcast(CompletionTime);
    }
}

//----------------------------------------------------------------------------------------
// МЕТОДЫ ОТСЛЕЖИВАНИЯ ВЫСОТЫ
//----------------------------------------------------------------------------------------

void AWTowerGameState::UpdatePlayerHeight(float NewHeight)
{
    // Обновляем текущую высоту персонажа
    CurrentPlayerHeight = NewHeight;
    
    // Обновляем максимальную высоту, если текущая высота больше
    if (CurrentPlayerHeight > MaxPlayerHeight)
    {
        MaxPlayerHeight = CurrentPlayerHeight;
        
        // Вызываем делегат для уведомления о изменении максимальной высоты
        OnMaxHeightChanged.Broadcast(MaxPlayerHeight);
    }
}

// Добавить в конец файла
void AWTowerGameState::SaveCurrentProgress(const FString& LevelName)
{
    UWTowerGameInstance* GameInstance = Cast<UWTowerGameInstance>(GetWorld()->GetGameInstance());
    if (GameInstance)
    {
        // Сохраняем лучшее время и счет
        if (bGameCompleted)
        {
            GameInstance->UpdateBestCompletionTime(LevelName, CompletionTime);
        }

        GameInstance->UpdateBestScore(LevelName, Score);

        // Сохраняем прогресс
        GameInstance->SaveGame();
    }
}