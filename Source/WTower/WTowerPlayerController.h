#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WTowerPlayerController.generated.h"

/**
 * Custom player controller for WTower
 */
UCLASS()
class WTOWER_API AWTowerPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    // Constructor
    AWTowerPlayerController();
    
    // Called when the controller possesses a pawn
    virtual void OnPossess(APawn* InPawn) override;
    
    // Called when game starts
    virtual void BeginPlay() override;

protected:
    // Called to bind functionality to input
    virtual void SetupInputComponent() override;
};