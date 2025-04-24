#include "WDefeatMenuWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "../WUIManager.h"

void UWDefeatMenuWidget::InitializeMenu()
{
    Super::InitializeMenu();
    
    // Set the defeat reason text if available
    if (DefeatReasonText && !DefeatReason.IsEmpty())
    {
        DefeatReasonText->SetText(FText::FromString(DefeatReason));
    }
}

void UWDefeatMenuWidget::OnPlayAgainClicked()
{
    // Reload current level
    UGameplayStatics::OpenLevel(this, FName(*UGameplayStatics::GetCurrentLevelName(this)));
}

void UWDefeatMenuWidget::OnMainMenuClicked()
{
    // Return to main menu level
    UGameplayStatics::OpenLevel(this, FName("MainMenu"));
}

void UWDefeatMenuWidget::SetDefeatReason(const FString& Reason)
{
    DefeatReason = Reason;
    
    // Update text if widget is already initialized
    if (DefeatReasonText)
    {
        DefeatReasonText->SetText(FText::FromString(DefeatReason));
    }
}