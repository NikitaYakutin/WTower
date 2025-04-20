#include "WBaseMenuWidget.h"
#include <WTower/Menu/MenuGameMode.h>

void UWBaseMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    bIsMenuOpen = false;
    InitializeMenu();
}

void UWBaseMenuWidget::InitializeMenu()
{
    // Base implementation - can be overridden in child classes
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
    // Используем GameMode для закрытия текущего меню
    AMenuGameMode* MenuGameMode = Cast<AMenuGameMode>(GetWorld()->GetAuthGameMode());
    if (MenuGameMode)
    {
        MenuGameMode->CloseCurrentMenu();
    }
    else
    {
        // Если GameMode недоступен, просто удаляем себя из родительского элемента
        RemoveFromParent();
    }
}

void UWBaseMenuWidget::AddToViewportWithZOrder(int32 ZOrder)
{
    AddToViewport(ZOrder);
}