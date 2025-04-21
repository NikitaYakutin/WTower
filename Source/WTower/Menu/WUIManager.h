// WUIManager.h - Единый класс управления пользовательским интерфейсом
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget/WBaseMenuWidget.h" // Важно включить заголовок базового класса меню
#include "Framework/Commands/InputChord.h"
#include "WUIManager.generated.h"

// Предварительные объявления классов
class UWMainMenuWidget;
class UWPauseMenuWidget;
class UWSettingsMenuWidget;
class UWVictoryScreenWidget;
//class UWDefeatScreenWidget;
class UWBaseMenuWidget;
//class UWHUDWidget;
class APlayerController;

// Типы меню
UENUM(BlueprintType)
enum class EWMenuType : uint8
{
    None,       // Нет активного меню
    Main,       // Главное меню
    Settings,   // Настройки
    Pause,      // Пауза
    Victory,    // Экран победы
    Defeat      // Экран поражения
};

// Делегат для событий смены меню
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMenuChangedSignature, EWMenuType, OldMenuType, EWMenuType, NewMenuType);

/**
 * Класс для централизованного управления всеми UI элементами
 */
UCLASS(BlueprintType, Blueprintable)
class WTOWER_API UWUIManager : public UObject
{
    GENERATED_BODY()

public:
    UWUIManager();

    /**
     * Инициализация менеджера UI
     * @param InOwningController Контроллер игрока, который владеет этим менеджером
     */
    UFUNCTION(BlueprintCallable, Category = "UI")
    void Initialize(APlayerController* InOwningController);
    /**
 * Установка классов виджетов для UI менеджера
 */
    UFUNCTION(BlueprintCallable, Category = "UI")
    void SetWidgetClasses(
        TSubclassOf<class UWMainMenuWidget> InMainMenuWidgetClass,
        TSubclassOf<class UWPauseMenuWidget> InPauseMenuWidgetClass,
        TSubclassOf<class UWSettingsMenuWidget> InSettingsMenuWidgetClass,
        TSubclassOf<class UWVictoryScreenWidget> InVictoryScreenWidgetClass);
    /**
     * Показать указанное меню
     * @param MenuType Тип меню для отображения
     */
    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowMenu(EWMenuType MenuType);

    /**
     * Закрыть текущее меню
     */
    UFUNCTION(BlueprintCallable, Category = "UI")
    void CloseCurrentMenu();

    /**
     * Вернуться к предыдущему меню
     */
    UFUNCTION(BlueprintCallable, Category = "UI")
    void ReturnToPreviousMenu();

    /**
     * Переключить меню паузы (показать/скрыть)
     */
    UFUNCTION(BlueprintCallable, Category = "UI")
    void TogglePauseMenu();

    /**
     * Открыть меню настроек
     */
    UFUNCTION(BlueprintCallable, Category = "UI")
    void OpenSettings();

    /**
     * Показать экран победы
     */
    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowVictoryScreen();

    /**
     * Показать экран поражения
    // */
    //UFUNCTION(BlueprintCallable, Category = "UI")
    //void ShowDefeatScreen();

    ///**
    // * Показать/скрыть HUD
    // * @param bShow True для отображения, False для скрытия
    // */
    //UFUNCTION(BlueprintCallable, Category = "UI")
    //void SetHUDVisibility(bool bShow);

    /**
     * Обработать нажатие клавиши
     * @param Key Нажатая клавиша
     * @return True, если клавиша была обработана
     */
    UFUNCTION(BlueprintCallable, Category = "UI")
    bool HandleKeyPress(FKey Key);

    /**
     * Получить текущий тип меню
     * @return Текущий тип меню
     */
    UFUNCTION(BlueprintPure, Category = "UI")
    EWMenuType GetCurrentMenuType() const { return CurrentMenuType; }

    /**
     * Получить предыдущий тип меню
     * @return Предыдущий тип меню
     */
    UFUNCTION(BlueprintPure, Category = "UI")
    EWMenuType GetPreviousMenuType() const { return PreviousMenuType; }

    /**
     * Проверить, открыто ли меню указанного типа
     * @param MenuType Тип меню для проверки
     * @return True, если меню открыто
     */
    UFUNCTION(BlueprintPure, Category = "UI")
    bool IsMenuOpen(EWMenuType MenuType) const { return CurrentMenuType == MenuType; }

    /**
     * Проверить, открыто ли какое-либо меню
     * @return True, если открыто какое-либо меню
     */
    UFUNCTION(BlueprintPure, Category = "UI")
    bool IsAnyMenuOpen() const { return CurrentMenuType != EWMenuType::None; }

    /** Делегат, вызываемый при смене меню */
    UPROPERTY(BlueprintAssignable, Category = "UI|Events")
    FOnMenuChangedSignature OnMenuChanged;
    // Классы виджетов
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Widgets")
    TSubclassOf<UWMainMenuWidget> MainMenuWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Widgets")
    TSubclassOf<UWPauseMenuWidget> PauseMenuWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Widgets")
    TSubclassOf<UWSettingsMenuWidget> SettingsMenuWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Widgets")
    TSubclassOf<UWVictoryScreenWidget> VictoryScreenWidgetClass;
private:
    /**
     * Настроить ввод для интерфейса
     */
    void SetupUIInput();

    /**
     * Настроить ввод для игры
     */
    void SetupGameInput();

    /**
     * Получить или создать виджет для указанного типа меню
     * @param MenuType Тип меню
     * @return Виджет для указанного типа меню
     */
    UWBaseMenuWidget* GetOrCreateWidget(EWMenuType MenuType) const;

    /**
     * Скрыть все меню
     */
    void HideAllMenus();

private:


    //UPROPERTY(EditDefaultsOnly, Category = "UI|Widgets")
    //TSubclassOf<UWDefeatScreenWidget> DefeatScreenWidgetClass;

    //UPROPERTY(EditDefaultsOnly, Category = "UI|Widgets")
    //TSubclassOf<UWHUDWidget> HUDWidgetClass;

    // Текущие экземпляры виджетов
    UPROPERTY()
    mutable UWMainMenuWidget* MainMenuWidget;

    UPROPERTY()
    mutable UWPauseMenuWidget* PauseMenuWidget;

    UPROPERTY()
    mutable UWSettingsMenuWidget* SettingsMenuWidget;

    UPROPERTY()
    mutable UWVictoryScreenWidget* VictoryScreenWidget;

    //UPROPERTY()
    //mutable UWDefeatScreenWidget* DefeatScreenWidget;

    //UPROPERTY()
    //UWHUDWidget* HUDWidget;

    // Текущий тип меню
    UPROPERTY()
    EWMenuType CurrentMenuType;

    // Предыдущий тип меню
    UPROPERTY()
    EWMenuType PreviousMenuType;

    // Контроллер игрока, владеющий этим менеджером
    UPROPERTY()
    APlayerController* OwningController;

    //// Флаг видимости HUD
    //UPROPERTY()
    //bool bHUDVisible;
};