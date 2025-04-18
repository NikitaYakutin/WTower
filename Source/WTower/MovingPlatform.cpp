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

    // Определяем ось движения на основе выбранного значения
    FVector DirectionVector;
    switch (MovementAxis)
    {
    case EPlatformMovementAxis::XAxis:
        DirectionVector = FVector(1.0f, 0.0f, 0.0f);
        break;
    case EPlatformMovementAxis::YAxis:
        DirectionVector = FVector(0.0f, 1.0f, 0.0f);
        break;
    case EPlatformMovementAxis::ZAxis:
        DirectionVector = FVector(0.0f, 0.0f, 1.0f);
        break;
    default:
        DirectionVector = FVector(0.0f, 1.0f, 0.0f);
        break;
    }

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
    FVector NewLocation = CurrentLocation + DirectionVector * MovementSpeed * GetMovementDirection() * DeltaTime;
    SetActorLocation(NewLocation);
}

void AMovingPlatform::ResumePlatformMovement()
{
    bIsPaused = false;
}