#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WBaseMenuWidget.generated.h"

/**
 * Base class for all menu widgets in the game
 */
UCLASS()
class WTOWER_API UWBaseMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // Initialize menu (called when the menu is shown)
    UFUNCTION(BlueprintCallable, Category = "Menu")
    virtual void InitializeMenu();
    
    // Reference to the UI manager
    UPROPERTY()
    class UWUIManager* UIManager;
};