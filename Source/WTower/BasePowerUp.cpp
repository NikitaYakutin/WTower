// BasePowerUp.cpp
#include "BasePowerUp.h"
#include "Components/SphereComponent.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Audio/WAudioManagerActor.h"
// Инициализация статических свойств
TMap<EPowerUpType, FLinearColor> ABasePowerUp::PowerUpColors;
bool ABasePowerUp::bColorsInitialized = false;

ABasePowerUp::ABasePowerUp()
{
    PrimaryActorTick.bCanEverTick = true;

    // Создаем компоненты
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    RootComponent = CollisionComponent;
    CollisionComponent->SetSphereRadius(50.0f);
    CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // Значения по умолчанию
    PowerUpType = EPowerUpType::ExtraJump;
    Duration = 5.0f;
    Strength = 1.0f;

    // Визуальные эффекты
    RotationSpeed = 90.0f;
    HoverAmplitude = 10.0f;
    HoverFrequency = 2.0f;
    TimeElapsed = 0.0f;

    // Инициализируем цвета усилений
    InitializePowerUpColors();
}

void ABasePowerUp::InitializePowerUpColors()
{
    if (!bColorsInitialized)
    {
        PowerUpColors.Add(EPowerUpType::ExtraJump, FLinearColor(0.0f, 1.0f, 0.0f));        // Зеленый
        PowerUpColors.Add(EPowerUpType::SpeedBoost, FLinearColor(0.0f, 0.8f, 1.0f));       // Голубой
        PowerUpColors.Add(EPowerUpType::Magnet, FLinearColor(0.8f, 0.2f, 1.0f));           // Фиолетовый
        PowerUpColors.Add(EPowerUpType::ScoreBonus, FLinearColor(1.0f, 0.5f, 0.5f));       // Розовый
        PowerUpColors.Add(EPowerUpType::Victory, FLinearColor(1.0f, 1.0f, 0.0f));          // Золотой
        bColorsInitialized = true;
    }
}

FLinearColor ABasePowerUp::GetColorForPowerUpType(EPowerUpType Type)
{
    InitializePowerUpColors();

    const FLinearColor* Color = PowerUpColors.Find(Type);
    if (Color)
    {
        return *Color;
    }

    return FLinearColor(1.0f, 1.0f, 1.0f); // Белый цвет по умолчанию
}

void ABasePowerUp::BeginPlay()
{
    Super::BeginPlay();

    // Сохраняем начальное положение для эффекта парения
    InitialLocation = GetActorLocation();

    // Подписываемся на событие пересечения
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABasePowerUp::OnOverlapBegin);

    // Обновляем визуальный стиль
    UpdateVisuals();
}

void ABasePowerUp::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Вращение
    FRotator NewRotation = GetActorRotation();
    NewRotation.Yaw += RotationSpeed * DeltaTime;
    SetActorRotation(NewRotation);

    // Эффект парения
    TimeElapsed += DeltaTime;
    float HoverOffset = HoverAmplitude * FMath::Sin(TimeElapsed * HoverFrequency);

    FVector NewLocation = InitialLocation;
    NewLocation.Z += HoverOffset;
    SetActorLocation(NewLocation);
}

void ABasePowerUp::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    // Проверяем, что пересечение произошло с игроком
    APlayerCharacter* Character = Cast<APlayerCharacter>(OtherActor);
    if (Character)
    {
        // Визуальные эффекты
        if (PickupSound)
        {
            AWAudioManagerActor* AudioManager = Cast<AWAudioManagerActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AWAudioManagerActor::StaticClass()));
            if (AudioManager)
            {
                AudioManager->PlaySoundAtLocation(PickupSound, GetActorLocation());
            }
        }

        if (PickupEffect)
        {
            UGameplayStatics::SpawnEmitterAtLocation(
                GetWorld(),
                PickupEffect,
                GetActorLocation(),
                FRotator::ZeroRotator,
                true
            );
        }

        // Применяем эффект усиления
        ApplyPowerUpEffect(Character);

        // Уничтожаем объект усиления
        Destroy();
    }
}

void ABasePowerUp::UpdateVisuals()
{
    UMaterialInstanceDynamic* DynamicMaterial = nullptr;

    // Устанавливаем базовую сферическую геометрию если не установлена
    if (!MeshComponent->GetStaticMesh())
    {
        UStaticMesh* SphereMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere.Sphere"));
        if (SphereMesh)
        {
            MeshComponent->SetStaticMesh(SphereMesh);
        }
    }

    // Создаем динамический материал
    if (MeshComponent->GetMaterial(0))
    {
        DynamicMaterial = UMaterialInstanceDynamic::Create(MeshComponent->GetMaterial(0), this);
    }
    else
    {
        // Пробуем создать базовый эмиссионный материал
        UMaterial* DefaultMat = LoadObject<UMaterial>(nullptr, TEXT("/Engine/EngineMaterials/DefaultLitEmissiveMaterial.DefaultLitEmissiveMaterial"));
        if (DefaultMat)
        {
            DynamicMaterial = UMaterialInstanceDynamic::Create(DefaultMat, this);
        }
    }

    // Устанавливаем цвет на основе типа усиления
    if (DynamicMaterial)
    {
        FLinearColor Color = GetColorForPowerUpType(PowerUpType);

        // Применяем цвет к материалу
        DynamicMaterial->SetVectorParameterValue(TEXT("Color"), Color);
        DynamicMaterial->SetVectorParameterValue(TEXT("EmissiveColor"), Color * 5.0f);
        DynamicMaterial->SetScalarParameterValue(TEXT("Emissive"), 5.0f);
        MeshComponent->SetMaterial(0, DynamicMaterial);
    }
}

void ABasePowerUp::ApplyPowerUpEffect_Implementation(AActor* Target)
{

    // Базовая реализация - ничего не делает
    // Этот метод переопределяется в конкретных усилениях
    UE_LOG(LogTemp, Warning, TEXT("Base power-up effect applied to %s"), *Target->GetName());
}
