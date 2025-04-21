#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MenuPlayerController.generated.h"

class UWUIManager;

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

    // Установить UIManager
    UFUNCTION(BlueprintCallable, Category = "UI")
    void SetUIManager(UWUIManager* InUIManager);

    // Получить UIManager
    UFUNCTION(BlueprintPure, Category = "UI")
    UWUIManager* GetUIManager() const;

private:
    // Ссылка на UIManager
    UPROPERTY()
    UWUIManager* UIManager;
};