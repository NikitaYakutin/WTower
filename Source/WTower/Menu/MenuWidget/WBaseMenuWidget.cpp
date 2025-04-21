// WBaseMenuWidget.cpp - Базовый класс для всех меню в игре
#include "WBaseMenuWidget.h"
#include "../WUIManager.h"
#include "../MenuGameMode.h"


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
    if (UIManager)
    {
        // Если у нас есть UIManager, используем его для закрытия меню
        UIManager->CloseCurrentMenu();
    }
    else
    {
        // Резервный вариант: используем GameMode
        AMenuGameMode* MenuGameMode = Cast<AMenuGameMode>(GetWorld()->GetAuthGameMode());
        if (MenuGameMode)
        {
            MenuGameMode->CloseCurrentMenu();
        }
        else
        {
            // Если ничего не сработало, просто удаляем себя
            RemoveFromParent();
        }
    }

    bIsMenuOpen = false;
    OnMenuClosed.Broadcast();
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