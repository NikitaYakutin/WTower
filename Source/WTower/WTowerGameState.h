#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "WTowerGameState.generated.h"

/**
 * Игровое состояние для WTower с отслеживанием очков, времени и высоты персонажа
 */
UCLASS()
class WTOWER_API AWTowerGameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    // Конструктор
    AWTowerGameState();

    // Вызывается каждый кадр
    virtual void Tick(float DeltaTime) override;

    //----------------------------------------------------------------------------------------
    // СИСТЕМА ОЧКОВ
    //----------------------------------------------------------------------------------------
    
    // Получить текущий счет игрока
    UFUNCTION(BlueprintCallable, Category = "Игровая статистика")
    int32 GetScore() const { return Score; }
    
    // Добавить очки к счету игрока
    UFUNCTION(BlueprintCallable, Category = "Игровая статистика")
    void AddScore(int32 Points);
    
    // Сбросить счет игрока на ноль
    UFUNCTION(BlueprintCallable, Category = "Игровая статистика")
    void ResetScore();

    //----------------------------------------------------------------------------------------
    // СИСТЕМА ТАЙМЕРА
    //----------------------------------------------------------------------------------------
    
    // Получить текущее время игры в секундах
    UFUNCTION(BlueprintCallable, Category = "Игровая статистика")
    float GetGameTime() const { return GameTime; }
    
    // Получить итоговое время прохождения (если игра завершена)
    UFUNCTION(BlueprintCallable, Category = "Игровая статистика")
    float GetCompletionTime() const { return CompletionTime; }
    
    // Отметить игру как завершенную и зафиксировать время прохождения
    UFUNCTION(BlueprintCallable, Category = "Игровая статистика")
    void SetGameCompleted();
    
    // Проверить, завершена ли игра
    UFUNCTION(BlueprintCallable, Category = "Игровая статистика")
    bool IsGameCompleted() const { return bGameCompleted; }

    //----------------------------------------------------------------------------------------
    // СИСТЕМА ОТСЛЕЖИВАНИЯ ВЫСОТЫ
    //----------------------------------------------------------------------------------------
    
    // Получить максимальную достигнутую высоту персонажа
    UFUNCTION(BlueprintCallable, Category = "Игровая статистика")
    float GetPlayerMaxHeight() const { return MaxPlayerHeight; }
    
    // Получить текущую высоту персонажа
    UFUNCTION(BlueprintCallable, Category = "Игровая статистика")
    float GetPlayerCurrentHeight() const { return CurrentPlayerHeight; }
    
    // Обновить текущую высоту персонажа
    UFUNCTION(BlueprintCallable, Category = "Игровая статистика")
    void UpdatePlayerHeight(float NewHeight);

protected:
    // Вызывается при начале игры
    virtual void BeginPlay() override;

private:
    //----------------------------------------------------------------------------------------
    // ПЕРЕМЕННЫЕ СОСТОЯНИЯ
    //----------------------------------------------------------------------------------------
    
    // Текущий счет игрока
    UPROPERTY(VisibleAnywhere, Category = "Игровая статистика")
    int32 Score;

    // Таймер игры в секундах
    UPROPERTY(VisibleAnywhere, Category = "Игровая статистика")
    float GameTime;
    
    // Время завершения игры (если игра была завершена)
    UPROPERTY(VisibleAnywhere, Category = "Игровая статистика")
    float CompletionTime;
    
    // Флаг завершения игры
    UPROPERTY(VisibleAnywhere, Category = "Игровая статистика")
    bool bGameCompleted;
    
    // Максимальная достигнутая высота персонажа
    UPROPERTY(VisibleAnywhere, Category = "Игровая статистика")
    float MaxPlayerHeight;
    
    // Текущая высота персонажа
    UPROPERTY(VisibleAnywhere, Category = "Игровая статистика")
    float CurrentPlayerHeight;
    
    //----------------------------------------------------------------------------------------
    // ДЕЛЕГАТЫ ДЛЯ СОБЫТИЙ
    //----------------------------------------------------------------------------------------
    
    // Делегат для оповещения о изменении счета
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int32, NewScore);
    
public:
    // Публичный делегат для оповещения о изменении счета
    UPROPERTY(BlueprintAssignable, Category = "Игровая статистика")
    FOnScoreChanged OnScoreChanged;
    
    // Делегат для оповещения о изменении максимальной высоты
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHeightChanged, float, NewMaxHeight);
    
    // Публичный делегат для оповещения о изменении максимальной высоты
    UPROPERTY(BlueprintAssignable, Category = "Игровая статистика")
    FOnMaxHeightChanged OnMaxHeightChanged;
    
    // Делегат для оповещения о завершении игры
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameCompleted, float, CompletionTime);
    
    // Публичный делегат для оповещения о завершении игры
    UPROPERTY(BlueprintAssignable, Category = "Игровая статистика")
    FOnGameCompleted OnGameCompleted;
};