#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class TOWER_API APlayerCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Конструктор
    APlayerCharacter();

    // Вызывается каждый кадр
    virtual void Tick(float DeltaTime) override;

    // Настройка пользовательского ввода
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
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
};