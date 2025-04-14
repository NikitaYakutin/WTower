// SpeedBoostPowerUp.h
#pragma once

#include "CoreMinimal.h"
#include "BasePowerUp.h"
#include "SpeedBoostPowerUp.generated.h"

UCLASS()
class WTOWER_API ASpeedBoostPowerUp : public ABasePowerUp
{
    GENERATED_BODY()

public:
    ASpeedBoostPowerUp();

    // Специфические настройки для SpeedBoost
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed Boost")
    float SpeedMultiplier;

    // Эффект следа для быстрого движения
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    UParticleSystem* SpeedEffectTemplate;

protected:
    // Переопределяем метод применения усиления
    virtual void ApplyPowerUpEffect_Implementation(AActor* Target) override;
};