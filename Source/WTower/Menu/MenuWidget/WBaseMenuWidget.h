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
    // Initialize the widget
    UFUNCTION(BlueprintCallable, Category = "Menu")
    virtual void InitializeMenu();

    // Open this menu
    UFUNCTION(BlueprintCallable, Category = "Menu")
    virtual void OpenMenu();

    // Close this menu
    UFUNCTION(BlueprintCallable, Category = "Menu")
    virtual void CloseMenu();
    
    // Add to viewport with specific Z-Order
    UFUNCTION(BlueprintCallable, Category = "Menu")
    void AddToViewportWithZOrder(int32 ZOrder);

protected:
    // Override to implement additional initialization
    virtual void NativeConstruct() override;

    // Flag indicating if the menu is currently open
    UPROPERTY(BlueprintReadOnly, Category = "Menu")
    bool bIsMenuOpen;


};