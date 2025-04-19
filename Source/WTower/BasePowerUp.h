#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystemComponent.h"
#include "BasePowerUp.generated.h"

// Типы усилений
UENUM(BlueprintType)
enum class EPowerUpType : uint8
{
    None UMETA(DisplayName = "None"),
    ExtraJump UMETA(DisplayName = "Extra Jump"),
    SpeedBoost UMETA(DisplayName = "Speed Boost"),
    Magnet UMETA(DisplayName = "Magnet"),
    ScoreBonus UMETA(DisplayName = "Score Bonus"),
    Victory UMETA(DisplayName = "Victory Item")
};

UCLASS(Abstract)
class WTOWER_API ABasePowerUp : public AActor
{
    GENERATED_BODY()

public:
    ABasePowerUp();
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    USoundBase* ExpireSound;
    // Основные компоненты
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class USphereComponent* CollisionComponent;

    // Настройки усиления
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power-Up")
    EPowerUpType PowerUpType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power-Up")
    float Duration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power-Up")
    float Strength;

    // Визуальные настройки
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    float RotationSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    float HoverAmplitude;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    float HoverFrequency;

    // Визуальные эффекты
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    UParticleSystem* PickupEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    USoundBase* PickupSound;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // Обработка столкновений
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    // Обновление визуального стиля на основе типа усиления
    virtual void UpdateVisuals();

    // Применение эффекта усиления к актору
    UFUNCTION(BlueprintNativeEvent, Category = "Power-Up")
    void ApplyPowerUpEffect(AActor* Target);
    virtual void ApplyPowerUpEffect_Implementation(AActor* Target);

    // Хранение начальной позиции для эффекта парения
    FVector InitialLocation;
    float TimeElapsed;

    // Статические методы для цветов
    static TMap<EPowerUpType, FLinearColor> PowerUpColors;
    static bool bColorsInitialized;
    static void InitializePowerUpColors();
    static FLinearColor GetColorForPowerUpType(EPowerUpType Type);
};