// JumpBoostPowerUp.h
#pragma once

#include "CoreMinimal.h"
#include "BasePowerUp.h"
#include "JumpBoostPowerUp.generated.h"

UCLASS()
class WTOWER_API AJumpBoostPowerUp : public ABasePowerUp
{
    GENERATED_BODY()

public:
    AJumpBoostPowerUp();

    // Специфические настройки для JumpBoost
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump Boost")
    float JumpHeightMultiplier;

    // Частицы эффекта для прыжка
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    UParticleSystem* JumpEffectTemplate;

protected:
    // Переопределяем метод применения усиления
    virtual void ApplyPowerUpEffect_Implementation(AActor* Target) override;
};