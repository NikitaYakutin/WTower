// WBaseMenuWidget.h - Базовый класс для всех меню в игре
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Framework/Commands/InputChord.h" // Для FKey
#include "WBaseMenuWidget.generated.h"

// Forward declarations
class UWUIManager;


// Делегат для события закрытия меню
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMenuClosedSignature);

/**
 * Базовый класс для всех виджетов меню
 */
UCLASS()
class WTOWER_API UWBaseMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    /** Ссылка на UIManager */
    UPROPERTY()
    UWUIManager* UIManager;

    // Конструктор
    UWBaseMenuWidget(const FObjectInitializer& ObjectInitializer);

    // Native функции вместо BlueprintImplementableEvent
    UFUNCTION(BlueprintCallable, Category = "UI")
    virtual void InitializeMenu();

    UFUNCTION(BlueprintCallable, Category = "UI")
    virtual void OpenMenu();

    UFUNCTION(BlueprintCallable, Category = "UI")
    virtual void CloseMenu();



    // Add to viewport with specific Z-Order
    UFUNCTION(BlueprintCallable, Category = "UI")
    void AddToViewportWithZOrder(int32 ZOrder);

    /**
     * Вернуться к предыдущему меню
     */
    UFUNCTION(BlueprintCallable, Category = "UI")
    void ReturnToPreviousMenu();

    /**
     * Обработать нажатие клавиши
     * @param Key Нажатая клавиша
     * @return True, если клавиша была обработана
     */
    UFUNCTION(BlueprintNativeEvent, Category = "UI")
    bool HandleKeyPress(FKey Key);
    virtual bool HandleKeyPress_Implementation(FKey Key) { return false; }

    /** Делегат, вызываемый при закрытии меню */
    UPROPERTY(BlueprintAssignable, Category = "UI|Events")
    FOnMenuClosedSignature OnMenuClosed;

protected:
    // Override to implement additional initialization
    virtual void NativeConstruct() override;

    // Flag indicating if the menu is currently open
    UPROPERTY(BlueprintReadOnly, Category = "Menu")
    bool bIsMenuOpen;
};