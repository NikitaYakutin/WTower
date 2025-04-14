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

    // Дополнительные настройки пружинящей платформы
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Bounce")
    bool bScaleOnBounce;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Bounce")
    float BounceVisualStrength;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Bounce")
    bool bPlayEffectsOnBounce;

protected:
    virtual void BeginPlay() override;
    
    // Переопределяем метод отскока
    virtual void BouncePlatform(AActor* Activator) override;
    virtual void ActivatePlatform(AActor* Activator) override;

private:
    FTimerHandle BounceVisualTimerHandle;
};