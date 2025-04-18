// BasePlatform.h (упрощенная версия)
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "BasePlatform.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

// Типы платформ
UENUM(BlueprintType)
enum class EPlatformType : uint8
{
    Normal UMETA(DisplayName = "Normal"),
    Moving UMETA(DisplayName = "Moving"),
    Breakable UMETA(DisplayName = "Breakable"),
    Bouncy UMETA(DisplayName = "Bouncy")
};
UENUM(BlueprintType)
enum class EPlatformMovementAxis : uint8
{
    XAxis UMETA(DisplayName = "X Axis"),
    YAxis UMETA(DisplayName = "Y Axis"),
    ZAxis UMETA(DisplayName = "Z Axis (Vertical)")
};

UCLASS(Abstract, Blueprintable)
class WTOWER_API ABasePlatform : public AActor
{
    GENERATED_BODY()

public:
    // Конструктор
    ABasePlatform();

    // Компоненты
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Platform")
    UStaticMeshComponent* PlatformMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Platform")
    UBoxComponent* TopCollision;

    // Настройки платформы
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
    EPlatformType PlatformType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Movement")
    float MovementRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Movement")
    float MovementSpeed;

    // Замените переменную MoveHorizontal
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Movement")
    EPlatformMovementAxis MovementAxis;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Break")
    float BreakDelay;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Bounce")
    float BounceMultiplier;

    // Отладочные настройки
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    bool bShowDebugInfo;

protected:
    // Методы жизненного цикла
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // Метод взаимодействия с игроком
    UFUNCTION()
    virtual void OnPlayerLanded(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
        const FHitResult& SweepResult);

    // Методы поведения платформы, которые можно переопределить в производных классах
    UFUNCTION(BlueprintCallable, Category = "Platform")
    virtual void ActivatePlatform(AActor* Activator);

    UFUNCTION(BlueprintCallable, Category = "Platform")
    virtual void BreakPlatform();

    UFUNCTION(BlueprintCallable, Category = "Platform")
    virtual void MovePlatform(float DeltaTime);

    UFUNCTION(BlueprintCallable, Category = "Platform")
    virtual void BouncePlatform(AActor* Activator);

    // Геттеры для защищенных полей
    UFUNCTION(BlueprintPure, Category = "Platform")
    FVector GetInitialPosition() const { return InitialPosition; }

    UFUNCTION(BlueprintPure, Category = "Platform")
    float GetMovementDirection() const { return MovementDirection; }

    UFUNCTION(BlueprintCallable, Category = "Platform")
    void SetMovementDirection(float Direction) { MovementDirection = Direction; }

private:
    FVector InitialPosition;
    float MovementDirection;
};