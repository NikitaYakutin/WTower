#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BasePowerUp.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class WTOWER_API APlayerCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Конструктор
    APlayerCharacter();

    // Вызывается каждый кадр
    virtual void Tick(float DeltaTime) override;

    // Настройка пользовательского ввода
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void AddControllerYawInput(float Value) override;
    virtual void AddControllerPitchInput(float Value) override;

    // Вызывается при начале игры
    virtual void BeginPlay() override;

    // Функции перемещения
    void MoveForward(float Value);
    void MoveRight(float Value);

    // Функции управления камерой
    void MouseTurn(float Rate);
    void MouseLookUp(float Rate);

    // Обработка прыжков
    virtual void Landed(const FHitResult& Hit) override;

    UFUNCTION()
    void PerformJump();

    // Таймер автоматических прыжков
    FTimerHandle JumpTimerHandle;

    // Звуковые эффекты
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Аудио")
    USoundBase* JumpSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Аудио")
    USoundBase* LandSound;

    // Вспомогательный метод для воспроизведения звуков
    void PlayCharacterSound(USoundBase* Sound);
    //----------------------------------------------------------------------------------------
   // МЕТОДЫ УСИЛЕНИЙ
   //----------------------------------------------------------------------------------------
 // Метод для уведомления игрока об активации усиления
    UFUNCTION(BlueprintImplementableEvent, Category = "Power-Ups")
    void NotifyPowerUpActivated(EPowerUpType PowerUpType, float Duration);

    // Метод для уведомления игрока о деактивации усиления
    UFUNCTION(BlueprintImplementableEvent, Category = "Power-Ups")
    void NotifyPowerUpDeactivated(EPowerUpType PowerUpType);

    // Метод для включения визуального отображения активного усиления
    UFUNCTION(BlueprintCallable, Category = "Power-Ups")
    void DisplayActivePowerUp(EPowerUpType PowerUpType, float Duration);

    //----------------------------------------------------------------------------------------
   // МЕТОДЫ ИГРОВОЙ СТАТИСТИКИ
   //----------------------------------------------------------------------------------------

   // Добавить очки игроку
    UFUNCTION(BlueprintCallable, Category = "Игровая статистика")
    void AddScore(int32 Points);

    // Получить текущий счет игрока
    UFUNCTION(BlueprintCallable, Category = "Игровая статистика")
    int32 GetScore() const;

    // Получить текущее время игры
    UFUNCTION(BlueprintCallable, Category = "Игровая статистика")
    float GetGameTime() const;

    // Получить максимальную высоту игрока
    UFUNCTION(BlueprintCallable, Category = "Игровая статистика")
    float GetMaxHeight() const;

    // Установить игру как завершенную
    UFUNCTION(BlueprintCallable, Category = "Игровая статистика")
    void CompleteGame();

    // Обновить высоту персонажа в GameState
    UFUNCTION(BlueprintCallable, Category = "Игровая статистика")
    void UpdateHeight();
public:
    // Компоненты камеры
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Камера")
    class USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Камера")
    class UCameraComponent* FollowCamera;

    // Настройки камеры
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Камера")
    float CameraSensitivity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Камера")
    float MinPitchAngle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Камера")
    float MaxPitchAngle;

    // Свойства прыжка
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Геймплей")
    float JumpPower;

    // Настройки масштабирования камеры
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Камера")
    float MinZoomDistance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Камера")
    float MaxZoomDistance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Камера")
    float ZoomStep;

    // Функция масштабирования камеры
    void ZoomCamera(float AxisValue);

private:
    FRotator CameraRotation;
    FVector LastMovementInput;
    FVector GetDirectionFromCamera(EAxis::Type Axis) const;
    //----------------------------------------------------------------------------------------
// МЕТОДЫ УСИЛЕНИЙ
//----------------------------------------------------------------------------------------

        // Для хранения активных усилений и их таймеров
    UPROPERTY()
    TMap<EPowerUpType, FTimerHandle> ActivePowerUpTimers;

    // Для отладки и UI, отслеживаем активные усиления
    UPROPERTY()
    TMap<EPowerUpType, bool> ActivePowerUps;
};