#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MenuPlayerController.generated.h"

/**
 * PlayerController для уровня главного меню
 */
UCLASS()
class WTOWER_API AMenuPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AMenuPlayerController();
    
    virtual void BeginPlay() override;
};