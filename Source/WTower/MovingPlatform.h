// MovingPlatform.h
#pragma once

#include "CoreMinimal.h"
#include "BasePlatform.h"
#include "MovingPlatform.generated.h"

UCLASS()
class WTOWER_API AMovingPlatform : public ABasePlatform
{
    GENERATED_BODY()

public:
    AMovingPlatform();

    // Дополнительные настройки движения
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Movement")
    bool bPauseAtEndpoints;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Movement")
    float PauseDuration;

protected:
    virtual void BeginPlay() override;
    virtual void MovePlatform(float DeltaTime) override;

private:
    FTimerHandle PauseTimerHandle;
    bool bIsPaused;

    void ResumePlatformMovement();
};