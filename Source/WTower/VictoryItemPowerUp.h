// VictoryItemPowerUp.h
#pragma once

#include "CoreMinimal.h"
#include "BasePowerUp.h"
#include "VictoryItemPowerUp.generated.h"

UCLASS()
class WTOWER_API AVictoryItemPowerUp : public ABasePowerUp
{
    GENERATED_BODY()

public:
    AVictoryItemPowerUp();

    // Очки начисляемые при победе
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Victory")
    int32 VictoryBonusScore;

    // Эффекты победы
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    UParticleSystem* VictoryEffect;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    USoundBase* VictorySound;

protected:
    // Переопределяем метод применения усиления
    virtual void ApplyPowerUpEffect_Implementation(AActor* Target) override;
};