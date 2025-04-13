#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WTowerGameMode.generated.h"

/**
 * Basic GameMode for WTower that spawns the player character at PlayerStart locations
 */
UCLASS()
class WTOWER_API AWTowerGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    // Constructor
    AWTowerGameMode();
    
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    
    // Override FindPlayerStart to customize player spawning (if needed)
    virtual AActor* FindPlayerStart_Implementation(AController* Player, const FString& IncomingName) override;
};