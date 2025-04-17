// ScoreBoostPowerUp.h
#pragma once

#include "CoreMinimal.h"
#include "BasePowerUp.h"
#include "ScoreBoostPowerUp.generated.h"

UCLASS()
class WTOWER_API AScoreBoostPowerUp : public ABasePowerUp
{
    GENERATED_BODY()

public:
    AScoreBoostPowerUp();

    // Очки начисляемые при подборе
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score Boost")
    int32 ScoreBonus;

    // Частицы эффекта для очков
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    UParticleSystem* ScoreEffectTemplate;

protected:
    // Переопределяем метод применения усиления
    virtual void ApplyPowerUpEffect_Implementation(AActor* Target) override;
};