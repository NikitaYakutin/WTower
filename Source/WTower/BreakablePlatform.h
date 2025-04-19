// BreakablePlatform.h
#pragma once

#include "CoreMinimal.h"
#include "BasePlatform.h"
#include "BreakablePlatform.generated.h"

UCLASS()
class WTOWER_API ABreakablePlatform : public ABasePlatform
{
    GENERATED_BODY()

public:
    ABreakablePlatform();
    UPROPERTY(EditDefaultsOnly, Category = "Audio")
    USoundBase* BreakSound;
    // Дополнительные настройки разрушаемой платформы
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Break")
    bool bShakeBeforeBreak;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Break")
    float ShakeIntensity;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Break")
    bool bPlayEffectsOnBreak;

protected:
    virtual void BeginPlay() override;
    
    // Переопределяем методы разрушаемой платформы
    virtual void ActivatePlatform(AActor* Activator) override;
    virtual void BreakPlatform() override;

private:
    FTimerHandle ShakeTimerHandle;
    bool bIsBreaking;
    
    void ShakePlatform();
};