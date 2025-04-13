#include "WTowerGameMode.h"
#include "PlayerCharacter.h"
#include "WTowerPlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

AWTowerGameMode::AWTowerGameMode()
{
    // Set default pawn class to our player character
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_PlayerCharacter"));
    if (PlayerPawnBPClass.Class != nullptr)
    {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }
    else
    {
        DefaultPawnClass = APlayerCharacter::StaticClass();
    }
    
    // Set default player controller class
    PlayerControllerClass = AWTowerPlayerController::StaticClass();
}

void AWTowerGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    UE_LOG(LogTemp, Log, TEXT("WTowerGameMode: Game started"));
}

AActor* AWTowerGameMode::FindPlayerStart_Implementation(AController* Player, const FString& IncomingName)
{
    // First try the default implementation which uses PlayerStart actors
    AActor* PlayerStart = Super::FindPlayerStart_Implementation(Player, IncomingName);
    
    // If no PlayerStart was found, find any PlayerStart in the world
    if (!PlayerStart)
    {
        UE_LOG(LogTemp, Warning, TEXT("No valid PlayerStart found, searching for any available PlayerStart"));
        
        // Find the first PlayerStart in the world
        for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
        {
            PlayerStart = *It;
            break;
        }
    }
    
    // Log where we're spawning the player
    if (PlayerStart)
    {
        UE_LOG(LogTemp, Log, TEXT("Player spawning at %s"), *PlayerStart->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No PlayerStart found in the level!"));
    }
    
    return PlayerStart;
}