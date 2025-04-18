// BasePlatform.cpp (упрощенная версия)
#include "BasePlatform.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

ABasePlatform::ABasePlatform()
{
    PrimaryActorTick.bCanEverTick = true;

    // Создаем корневой компонент
    PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
    RootComponent = PlatformMesh;

    if (PlatformMesh)
    {
        PlatformMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        PlatformMesh->SetCollisionResponseToAllChannels(ECR_Block);
    }

    // Создаем коллизию верхней части
    TopCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("TopCollision"));
    if (TopCollision && RootComponent)
    {
        TopCollision->SetupAttachment(RootComponent);
        TopCollision->SetBoxExtent(FVector(50.0f, 50.0f, 2.0f));
        TopCollision->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f));
        TopCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        TopCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
        TopCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    }

    // Значения по умолчанию
    PlatformType = EPlatformType::Normal;
    MovementRange = 200.0f;
    MovementSpeed = 100.0f;
    MovementAxis = EPlatformMovementAxis::YAxis; // По умолчанию
    BreakDelay = 0.5f;
    BounceMultiplier = 1.5f;
    MovementDirection = 1.0f;
    bShowDebugInfo = false;
}

void ABasePlatform::BeginPlay()
{
    Super::BeginPlay();

    // Сохраняем начальную позицию для движущихся платформ
    InitialPosition = GetActorLocation();

    // Регистрируем обработчик события пересечения
    TopCollision->OnComponentBeginOverlap.AddDynamic(this, &ABasePlatform::OnPlayerLanded);

    // Выводим отладочную информацию
    if (bShowDebugInfo)
    {
        FString TypeName;
        switch (PlatformType)
        {
        case EPlatformType::Normal: TypeName = "Normal"; break;
        case EPlatformType::Moving: TypeName = "Moving"; break;
        case EPlatformType::Breakable: TypeName = "Breakable"; break;
        case EPlatformType::Bouncy: TypeName = "Bouncy"; break;
        default: TypeName = "Unknown"; break;
        }
        UE_LOG(LogTemp, Display, TEXT("Platform type %s created at position %s"), *TypeName, *GetActorLocation().ToString());
    }
}

void ABasePlatform::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Обрабатываем движущиеся платформы
    if (PlatformType == EPlatformType::Moving)
    {
        MovePlatform(DeltaTime);
    }
}

void ABasePlatform::MovePlatform(float DeltaTime)
{
    FVector CurrentLocation = GetActorLocation();

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


    // Рассчитываем расстояние движения
    float Distance = FVector::Dist(CurrentLocation, InitialPosition);

    // Если мы достигли диапазона движения, меняем направление
    if (Distance >= MovementRange)
    {
        MovementDirection *= -1.0f;
    }

    // Применяем движение
    FVector NewLocation = CurrentLocation + DirectionVector * MovementSpeed * MovementDirection * DeltaTime;
    SetActorLocation(NewLocation);
}

void ABasePlatform::OnPlayerLanded(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    // Проверяем, что приземлившийся актор - это персонаж игрока
    APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
    if (Player)
    {
        UE_LOG(LogTemp, Display, TEXT("Игрок приземлился на платформу типа %d"), static_cast<int32>(PlatformType));

        // Обрабатываем разные типы платформ
        switch (PlatformType)
        {
        case EPlatformType::Breakable:
            // Планируем разрушение платформы
            if (GetWorld())
            {
                UE_LOG(LogTemp, Display, TEXT("Breakable platform: Starting destruction sequence in %f seconds"), BreakDelay);
                FTimerHandle TimerHandle;
                GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABasePlatform::BreakPlatform, BreakDelay, false);
            }
            break;

        case EPlatformType::Bouncy:
            // Применяем отскок к персонажу
            BouncePlatform(Player);
            break;

        default:
            // Обычные и движущиеся платформы обрабатываются стандартным приземлением персонажа
            break;
        }

        // Активируем платформу (для любого специфичного поведения платформы)
        ActivatePlatform(Player);
    }
}

void ABasePlatform::ActivatePlatform(AActor* Activator)
{
    // Базовая реализация ничего не делает - переопределите в производных классах
}

void ABasePlatform::BreakPlatform()
{
    UE_LOG(LogTemp, Display, TEXT("Платформа разрушается"));

    // Отключаем коллизию
    PlatformMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    TopCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // Делаем платформу невидимой
    PlatformMesh->SetVisibility(false);

    // Уничтожаем с задержкой, чтобы эффекты могли проиграться
    SetLifeSpan(2.0f);
}

void ABasePlatform::BouncePlatform(AActor* Activator)
{
    // Визуальный эффект сжатия и растяжения
    if (PlatformMesh)
    {
        // Сжимаем платформу
        FVector OriginalScale = PlatformMesh->GetRelativeScale3D();
        FVector CompressedScale = FVector(OriginalScale.X * 1.2f, OriginalScale.Y * 1.2f, OriginalScale.Z * 0.5f);
        PlatformMesh->SetRelativeScale3D(CompressedScale);

        // Возвращаем к исходному размеру с задержкой
        FTimerHandle ScaleTimerHandle;
        GetWorld()->GetTimerManager().SetTimer(ScaleTimerHandle, [this, OriginalScale]()
            {
                if (PlatformMesh)
                {
                    PlatformMesh->SetRelativeScale3D(OriginalScale);
                }
            }, 0.15f, false);
    }

    // Применяем дополнительный отскок к персонажу
    APlayerCharacter* Player = Cast<APlayerCharacter>(Activator);
    if (Player && Player->GetCharacterMovement())
    {
        UE_LOG(LogTemp, Display, TEXT("Bouncy platform: Applying bounce factor %f"), BounceMultiplier);

        // Сохраняем текущую скорость падения и увеличиваем её
        float OriginalJumpVelocity = Player->GetCharacterMovement()->JumpZVelocity;
        Player->GetCharacterMovement()->JumpZVelocity = OriginalJumpVelocity * BounceMultiplier;

        // Прикладываем импульс напрямую для более надежного эффекта
        Player->GetCharacterMovement()->Velocity = FVector(
            Player->GetCharacterMovement()->Velocity.X,
            Player->GetCharacterMovement()->Velocity.Y,
            Player->GetCharacterMovement()->JumpZVelocity
        );
        Player->Jump();

        // Возвращаем исходную скорость прыжка
        FTimerHandle ResetTimerHandle;
        GetWorld()->GetTimerManager().SetTimer(ResetTimerHandle, [Player, OriginalJumpVelocity]()
            {
                if (Player && Player->GetCharacterMovement())
                {
                    Player->GetCharacterMovement()->JumpZVelocity = OriginalJumpVelocity;
                }
            }, 0.5f, false);
    }
}