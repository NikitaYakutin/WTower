#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

//----------------------------------------------------------------------------------------
// КОНСТРУКТОР И ИНИЦИАЛИЗАЦИЯ
//----------------------------------------------------------------------------------------

APlayerCharacter::APlayerCharacter()
{
    // Включаем Tick
    PrimaryActorTick.bCanEverTick = true;

    // Настраиваем капсулу столкновения
    GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);
    GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetCapsuleComponent()->SetUseCCD(true); // Включаем непрерывное определение столкновений

    // Настраиваем компонент меша
    USkeletalMeshComponent* MeshComponent = GetMesh();
    MeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -96.0f));
    MeshComponent->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

    // Настраиваем компонент движения персонажа
    UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
    MovementComponent->bOrientRotationToMovement = false; // Мы будем управлять поворотом самостоятельно
    MovementComponent->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    MovementComponent->GravityScale = 1.8f;
    MovementComponent->JumpZVelocity = 1000.0f;
    MovementComponent->AirControl = 0.8f;
    MovementComponent->MaxWalkSpeed = 600.0f;
    MovementComponent->MaxAcceleration = 2048.0f;
    MovementComponent->BrakingDecelerationFalling = 2048.0f;
    MovementComponent->bUseControllerDesiredRotation = false;

    // Создаем стрелу камеры
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 400.0f;
    CameraBoom->bUsePawnControlRotation = false;
    CameraBoom->bInheritPitch = true;
    CameraBoom->bInheritYaw = true;
    CameraBoom->bInheritRoll = false;
    
    // Настройки стабилизации камеры
    CameraBoom->bEnableCameraLag = true;
    CameraBoom->CameraLagSpeed = 10.0f;
    CameraBoom->CameraLagMaxDistance = 50.0f;
    CameraBoom->bEnableCameraRotationLag = true;
    CameraBoom->CameraRotationLagSpeed = 10.0f;
    CameraBoom->CameraLagMaxTimeStep = 0.05f;
    CameraBoom->bDoCollisionTest = true;

    // Создаем камеру слежения
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    // Значения по умолчанию
    JumpPower = 850.0f;
    CameraSensitivity = 2.0f;
    MinPitchAngle = -80.0f;
    MaxPitchAngle = 80.0f;
    CameraRotation = FRotator::ZeroRotator;
    
    // Настройки масштабирования
    MinZoomDistance = 200.0f;
    MaxZoomDistance = 800.0f;
    ZoomStep = 50.0f;
    
    // Не используем поворот контроллера, мы будем управлять им самостоятельно
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;
}

//----------------------------------------------------------------------------------------
// МЕТОДЫ ЖИЗНЕННОГО ЦИКЛА
//----------------------------------------------------------------------------------------

void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    // Инициализируем первый прыжок с задержкой
    // Задержка необходима, чтобы дать персонажу время на инициализацию и стабилизацию
    // перед выполнением первого прыжка
    GetWorldTimerManager().SetTimer(
        JumpTimerHandle,
        this,
        &APlayerCharacter::PerformJump,
        0.5f,
        false
    );
    
    // Инициализируем вращение камеры из текущего вращения
    CameraRotation = CameraBoom->GetRelativeRotation();
}

void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // Плавно интерполируем вращение камеры
    FRotator CurrentRotation = CameraBoom->GetRelativeRotation();
    FRotator NewRotation = FMath::RInterpTo(CurrentRotation, CameraRotation, DeltaTime, 10.0f);
    CameraBoom->SetRelativeRotation(NewRotation);
    
    // Всегда поворачиваем персонажа в направлении взгляда камеры (по Yaw)
    FRotator CharacterRotation = GetActorRotation();
    CharacterRotation.Yaw = CameraRotation.Yaw;
    SetActorRotation(CharacterRotation);
}

//----------------------------------------------------------------------------------------
// НАСТРОЙКА ВВОДА
//----------------------------------------------------------------------------------------

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    // Привязки движения
    PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
    
    // Привязки управления камерой - всегда активны без необходимости зажимать ПКМ
    PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::MouseTurn);
    PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::MouseLookUp);
    
    // Привязка масштабирования
    PlayerInputComponent->BindAxis("CameraZoom", this, &APlayerCharacter::ZoomCamera);
}

//----------------------------------------------------------------------------------------
// ПЕРЕМЕЩЕНИЕ ПЕРСОНАЖА
//----------------------------------------------------------------------------------------

void APlayerCharacter::MoveForward(float Value)
{
    if ((Controller != nullptr) && (Value != 0.0f))
    {
        // Получаем направление "вперед" относительно камеры
        const FRotator Rotation = CameraRotation;
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        
        // Применяем движение
        AddMovementInput(Direction, Value);
        LastMovementInput.X = Value;
    }
    else
    {
        LastMovementInput.X = 0.0f;
    }
}

void APlayerCharacter::MoveRight(float Value)
{
    if ((Controller != nullptr) && (Value != 0.0f))
    {
        // Получаем направление "вправо" относительно камеры
        const FRotator Rotation = CameraRotation;
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        
        // Применяем движение
        AddMovementInput(Direction, Value);
        LastMovementInput.Y = Value;
    }
    else
    {
        LastMovementInput.Y = 0.0f;
    }
}

//----------------------------------------------------------------------------------------
// УПРАВЛЕНИЕ КАМЕРОЙ
//----------------------------------------------------------------------------------------

void APlayerCharacter::MouseTurn(float Rate)
{
    // Всегда активная орбитальная камера (не требуется зажатие ПКМ)
    if (Rate != 0.0f)
    {
        // Плавное вращение по Yaw с учетом чувствительности
        float DeltaYaw = Rate * CameraSensitivity;
        DeltaYaw = FMath::Clamp(DeltaYaw, -5.0f, 5.0f);
        CameraRotation.Yaw += DeltaYaw;
    }
}

void APlayerCharacter::MouseLookUp(float Rate)
{
    // Всегда активная орбитальная камера (не требуется зажатие ПКМ)
    if (Rate != 0.0f)
    {
        // Плавное вращение по Pitch с учетом чувствительности и ограничений
        float DeltaPitch = Rate * CameraSensitivity;
        DeltaPitch = FMath::Clamp(DeltaPitch, -5.0f, 5.0f);
        
        // Применяем ограничения Pitch
        CameraRotation.Pitch = FMath::Clamp(
            CameraRotation.Pitch + DeltaPitch,
            MinPitchAngle,
            MaxPitchAngle
        );
    }
}

void APlayerCharacter::ZoomCamera(float AxisValue)
{
    if (CameraBoom && AxisValue != 0.0f)
    {
        // Вычисляем новую длину камеры (отрицательное значение для колеса мыши - приближение)
        float NewTargetArmLength = CameraBoom->TargetArmLength - (ZoomStep * AxisValue);
        
        // Ограничиваем минимальными и максимальными значениями
        NewTargetArmLength = FMath::Clamp(NewTargetArmLength, MinZoomDistance, MaxZoomDistance);
        
        // Устанавливаем новую длину
        CameraBoom->TargetArmLength = NewTargetArmLength;
    }
}

//----------------------------------------------------------------------------------------
// СИСТЕМА ПРЫЖКОВ
//----------------------------------------------------------------------------------------

void APlayerCharacter::Landed(const FHitResult& Hit)
{
    Super::Landed(Hit);
    
    // Воспроизводим звук приземления
    PlayCharacterSound(LandSound);
    
    // Планируем следующий прыжок с небольшой задержкой
    // Задержка в 0.1 секунды необходима для того, чтобы:
    // 1. Дать персонажу стабилизироваться после приземления
    // 2. Избежать "залипания" в земле из-за слишком раннего прыжка
    // 3. Создать естественную паузу между прыжками
    GetWorldTimerManager().SetTimer(
        JumpTimerHandle,
        this,
        &APlayerCharacter::PerformJump,
        0.1f,
        false
    );
}

void APlayerCharacter::PerformJump()
{
    // Проверяем, находимся ли мы на земле
    if (GetCharacterMovement()->IsMovingOnGround())
    {
        // Очищаем горизонтальную скорость перед прыжком
        // для более контролируемой траектории
        FVector Velocity = GetCharacterMovement()->Velocity;
        Velocity.X = 0.0f;
        Velocity.Y = 0.0f;
        GetCharacterMovement()->Velocity = Velocity;
        
        // Воспроизводим звук прыжка
        PlayCharacterSound(JumpSound);
        
        // Вызываем базовую функцию Jump
        Jump();
        
        // Устанавливаем только вертикальную скорость для
        // стабильной траектории прыжка
        GetCharacterMovement()->Velocity = FVector(0.0f, 0.0f, JumpPower);
    }
}

//----------------------------------------------------------------------------------------
// ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ
//----------------------------------------------------------------------------------------

void APlayerCharacter::PlayCharacterSound(USoundBase* Sound)
{
    if (Sound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            this,
            Sound,
            GetActorLocation()
        );
    }
}