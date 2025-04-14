// NormalPlatform.h
#pragma once

#include "CoreMinimal.h"
#include "BasePlatform.h"
#include "NormalPlatform.generated.h"

UCLASS()
class WTOWER_API ANormalPlatform : public ABasePlatform
{
    GENERATED_BODY()

public:
    ANormalPlatform();

    // Дополнительные настройки нормальной платформы (если они понадобятся)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
    bool bApplyImpulseOnLanding;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
    float LandingImpulseStrength;

protected:
    virtual void BeginPlay() override;
    
    // Переопределяем метод активации платформы
    virtual void ActivatePlatform(AActor* Activator) override;
};