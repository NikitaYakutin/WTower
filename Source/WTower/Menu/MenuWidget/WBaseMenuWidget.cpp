// WBaseMenuWidget.cpp - Базовый класс для всех меню в игре
#include "WBaseMenuWidget.h"
#include "../WUIManager.h"
#include "../MenuGameMode.h"
#include <WTower/Menu/MenuPlayerController.h>


UWBaseMenuWidget::UWBaseMenuWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bIsMenuOpen = false;
}

void UWBaseMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    InitializeMenu();
}

void UWBaseMenuWidget::InitializeMenu()
{
    // Базовая реализация - может быть переопределена в дочерних классах

    // Если UIManager не установлен, попробуем получить его из GameMode
    if (!UIManager)
    {
        AMenuGameMode* MenuGameMode = Cast<AMenuGameMode>(GetWorld()->GetAuthGameMode());
        if (MenuGameMode)
        {
            UIManager = MenuGameMode->GetUIManager();
            if (!UIManager)
            {
                UE_LOG(LogTemp, Warning, TEXT("WBaseMenuWidget: UIManager not found in GameMode!"));
            }
        }
        else
        {
            // Попытаемся получить через контроллер
            AMenuPlayerController* PC = Cast<AMenuPlayerController>(GetOwningPlayer());
            if (PC)
            {
                UIManager = PC->GetUIManager();
                if (!UIManager)
                {
                    UE_LOG(LogTemp, Warning, TEXT("WBaseMenuWidget: UIManager not found in PlayerController!"));
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("WBaseMenuWidget: MenuGameMode not found for menu initialization!"));
            }
        }
    }
}

void UWBaseMenuWidget::OpenMenu()
{
    if (!bIsMenuOpen)
    {
        AddToViewportWithZOrder(100);
        bIsMenuOpen = true;
    }
}

void UWBaseMenuWidget::CloseMenu()
{
    // Отмечаем, что меню закрыто
    bIsMenuOpen = false;

    // Вызываем делегат закрытия меню
    OnMenuClosed.Broadcast();

    // Не вызываем UIManager->CloseCurrentMenu(), чтобы избежать рекурсии
    // Вместо этого просто удаляем виджет из родительского элемента
    RemoveFromParent();
}

void UWBaseMenuWidget::AddToViewportWithZOrder(int32 ZOrder)
{
    AddToViewport(ZOrder);
}

void UWBaseMenuWidget::ReturnToPreviousMenu()
{
    if (UIManager)
    {
        UIManager->ReturnToPreviousMenu();
    }
}