// MovingPlatform.cpp
#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
    // Устанавливаем тип по умолчанию как Moving
    PlatformType = EPlatformType::Moving;

    // Устанавливаем специализированные значения по умолчанию для движущейся платформы
    bPauseAtEndpoints = false;
    PauseDuration = 1.0f;
    bIsPaused = false;
}

void AMovingPlatform::BeginPlay()
{
    Super::BeginPlay();

    // Убеждаемся, что платформа установлена как тип Moving
    PlatformType = EPlatformType::Moving;
}

void AMovingPlatform::MovePlatform(float DeltaTime)
{
    // Если на паузе, не двигаться
    if (bIsPaused)
    {
        return;
    }

    FVector CurrentLocation = GetActorLocation();
    FVector StartPosition = GetInitialPosition();

    // Определяем ось движения
    FVector MovementAxis = MoveHorizontal ? FVector(0.0f, 1.0f, 0.0f) : FVector(1.0f, 0.0f, 0.0f);

    // Вычисляем расстояние от начальной точки
    float Distance = FVector::Dist(CurrentLocation, StartPosition);

    // Если мы достигли диапазона движения и должны сделать паузу в конечных точках
    if (Distance >= MovementRange && bPauseAtEndpoints)
    {
        // Направление будет изменено при возобновлении движения
        SetMovementDirection(GetMovementDirection() * -1.0f);

        // Начинаем паузу
        bIsPaused = true;

        // Настраиваем таймер для возобновления движения
        GetWorld()->GetTimerManager().SetTimer(
            PauseTimerHandle,
            this,
            &AMovingPlatform::ResumePlatformMovement,
            PauseDuration,
            false
        );

        return;
    }

    // Если мы достигли диапазона движения, но не должны делать паузу, просто изменяем направление
    if (Distance >= MovementRange)
    {
        SetMovementDirection(GetMovementDirection() * -1.0f);
    }

    // Применяем движение
    FVector NewLocation = CurrentLocation + MovementAxis * MovementSpeed * GetMovementDirection() * DeltaTime;
    SetActorLocation(NewLocation);
}

void AMovingPlatform::ResumePlatformMovement()
{
    bIsPaused = false;
}