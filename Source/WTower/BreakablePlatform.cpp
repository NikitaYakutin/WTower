// BreakablePlatform.cpp
#include "BreakablePlatform.h"
#include "PlayerCharacter.h"
#include "Materials/MaterialInstanceDynamic.h"
#include <Kismet/GameplayStatics.h>
#include "Audio/WAudioManagerActor.h"

ABreakablePlatform::ABreakablePlatform()
{
    // Устанавливаем тип по умолчанию как Breakable
    PlatformType = EPlatformType::Breakable;
    
    // Настройки по умолчанию
    bShakeBeforeBreak = true;
    ShakeIntensity = 2.0f;
    bPlayEffectsOnBreak = true;
    bIsBreaking = false;
    
    // Устанавливаем задержку разрушения
    BreakDelay = 0.5f;
}

void ABreakablePlatform::BeginPlay()
{
    Super::BeginPlay();
    
    // Убеждаемся, что платформа установлена как тип Breakable
    PlatformType = EPlatformType::Breakable;
}

void ABreakablePlatform::ActivatePlatform(AActor* Activator)
{
    // Проверяем, что платформа ещё не в процессе разрушения
    if (bIsBreaking)
    {
        return;
    }
    
    // Отмечаем платформу как начавшую разрушаться
    bIsBreaking = true;
    
    // Визуальная индикация начала разрушения
    if (PlatformMesh)
    {
        AWAudioManagerActor* AudioManager = Cast<AWAudioManagerActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AWAudioManagerActor::StaticClass()));
        if (AudioManager)
        {
            AudioManager->PlaySoundAtLocation(BreakSound, GetActorLocation());;
        }
        UMaterialInstanceDynamic* DynMaterial = Cast<UMaterialInstanceDynamic>(PlatformMesh->GetMaterial(0));
        if (!DynMaterial)
        {
            DynMaterial = UMaterialInstanceDynamic::Create(PlatformMesh->GetMaterial(0), this);
            PlatformMesh->SetMaterial(0, DynMaterial);
        }

        if (DynMaterial)
        {
            // Меняем цвет на красный для индикации
            DynMaterial->SetVectorParameterValue(TEXT("Color"), FLinearColor(1.0f, 0.2f, 0.2f));
        }

        // Добавляем покачивание платформы если нужно
        if (bShakeBeforeBreak)
        {
            GetWorld()->GetTimerManager().SetTimer(
                ShakeTimerHandle, 
                this, 
                &ABreakablePlatform::ShakePlatform, 
                0.05f, 
                true
            );
        }
    }
    
    // Планируем разрушение платформы
    if (GetWorld())
    {
        UE_LOG(LogTemp, Display, TEXT("Breakable platform: Starting destruction sequence in %f seconds"), BreakDelay);
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABreakablePlatform::BreakPlatform, BreakDelay, false);
    }
    
    // Вызываем базовую реализацию
    Super::ActivatePlatform(Activator);
}

void ABreakablePlatform::ShakePlatform()
{
    if (PlatformMesh && bIsBreaking)
    {
        float ShakeAmount = ShakeIntensity;
        FVector NewLocation = GetActorLocation() + FVector(
            FMath::RandRange(-ShakeAmount, ShakeAmount),
            FMath::RandRange(-ShakeAmount, ShakeAmount),
            0);
        SetActorLocation(NewLocation);
    }
}

void ABreakablePlatform::BreakPlatform()
{
    // Останавливаем таймер тряски
    GetWorld()->GetTimerManager().ClearTimer(ShakeTimerHandle);
    
    UE_LOG(LogTemp, Display, TEXT("Платформа разрушается"));

    // Визуальные эффекты если нужны
    if (bPlayEffectsOnBreak)
    {
        // Здесь можно добавить частицы, звуки и т.д.
        // Для примера:
        /*
        if (BreakEffect)
        {
            UGameplayStatics::SpawnEmitterAtLocation(
                GetWorld(),
                BreakEffect,
                GetActorLocation(),
                GetActorRotation()
            );
        }
        */
    }

    // Отключаем коллизию
    PlatformMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    TopCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // Делаем платформу невидимой
    PlatformMesh->SetVisibility(false);

    // Уничтожаем с задержкой
    SetLifeSpan(2.0f);
    
    // Вызываем базовую реализацию
    Super::BreakPlatform();
}