// NormalPlatform.cpp
#include "NormalPlatform.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

ANormalPlatform::ANormalPlatform()
{
    // Устанавливаем тип по умолчанию как Normal
    PlatformType = EPlatformType::Normal;
    
    // Настройки по умолчанию
    bApplyImpulseOnLanding = false;
    LandingImpulseStrength = 100.0f;
}

void ANormalPlatform::BeginPlay()
{
    Super::BeginPlay();
    
    // Убеждаемся, что платформа установлена как тип Normal
    PlatformType = EPlatformType::Normal;
}

void ANormalPlatform::ActivatePlatform(AActor* Activator)
{
    // Базовое поведение для нормальной платформы
    Super::ActivatePlatform(Activator);
    
    // Если нужно применить импульс при приземлении
    if (bApplyImpulseOnLanding)
    {
        APlayerCharacter* Player = Cast<APlayerCharacter>(Activator);
        if (Player && Player->GetCharacterMovement())
        {
            // Можно добавить небольшой импульс вверх
            FVector Impulse = FVector(0.0f, 0.0f, LandingImpulseStrength);
            Player->GetCharacterMovement()->AddImpulse(Impulse, true);
        }
    }
}