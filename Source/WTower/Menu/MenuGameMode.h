#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MenuGameMode.generated.h"


/**
 * GameMode для управления всеми меню
 */
UCLASS()
class WTOWER_API AMenuGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMenuGameMode();

    // Инициализация игры
    virtual void BeginPlay() override;




private:


};