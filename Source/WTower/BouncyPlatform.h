// BouncyPlatform.h
#pragma once

#include "CoreMinimal.h"
#include "BasePlatform.h"
#include "BouncyPlatform.generated.h"

UCLASS()
class WTOWER_API ABouncyPlatform : public ABasePlatform
{
    GENERATED_BODY()

public:
    ABouncyPlatform();
    UPROPERTY(EditDefaultsOnly, Category = "Audio")
    USoundBase* BouncySound;
    // Дополнительные настройки пружинящей платформы
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Bounce")
    bool bScaleOnBounce;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Bounce")
    float BounceVisualStrength;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Bounce")
    bool bPlayEffectsOnBounce;

protected:
    virtual void BeginPlay() override;
    // Оригинальный масштаб платформы
    TOptional<FVector> OriginalPlatformScale;

    // Таймер для восстановления масштаба
    FTimerHandle ScaleTimerHandle;
    // Время последней активации
    float LastActivationTime = 0.0f;

    // Минимальное время между активациями (секунды)
    UPROPERTY(EditAnywhere, Category = "Platform|Bounce")
    float CooldownTime = 0.1f;
    // Переопределяем метод отскока
    virtual void BouncePlatform(AActor* Activator) override;
    virtual void ActivatePlatform(AActor* Activator) override;

private:
    FTimerHandle BounceVisualTimerHandle;
};