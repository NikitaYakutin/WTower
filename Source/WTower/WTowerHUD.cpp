#include "WTowerHUD.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "WTowerPlayerController.h"
#include "Menu/WUIManager.h"

AWTowerHUD::AWTowerHUD()
{
    // Constructor
}

void AWTowerHUD::BeginPlay()
{
    Super::BeginPlay();

    // Debug log
    UE_LOG(LogTemp, Display, TEXT("WTowerHUD::BeginPlay - Initializing HUD"));

    // Get player controller
    AWTowerPlayerController* PC = Cast<AWTowerPlayerController>(GetOwningPlayerController());

    // Use UI Manager if available
    if (PC && PC->GetUIManager())
    {
        UE_LOG(LogTemp, Display, TEXT("Using UIManager to show HUD"));
        PC->GetUIManager()->ShowHUD();
    }
    // Legacy fallback - This is important if UIManager isn't properly initialized
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UIManager not available, falling back to direct HUD creation"));

        // Check if widget class is set
        if (!HUDWidgetClass)
        {
            // Load widget class programmatically
            HUDWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/Blueprints/Menu/WBP_GameHUD.WBP_GameHUD_C"));

            if (!HUDWidgetClass)
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to load HUD widget class!"));
                return;
            }
        }

        // Create and add widget
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (HUDWidgetClass && PlayerController)
        {
            CurrentHUDWidget = CreateWidget<UUserWidget>(PlayerController, HUDWidgetClass);
            if (CurrentHUDWidget)
            {
                CurrentHUDWidget->AddToViewport(0);
                UE_LOG(LogTemp, Display, TEXT("HUD widget added to viewport"));
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to create HUD widget!"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Missing HUDWidgetClass or PlayerController!"));
        }
    }
}