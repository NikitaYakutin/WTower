#include "WTowerGameMode.h"
#include "PlayerCharacter.h"
#include "WTowerPlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "WTowerGameState.h"
#include "WTowerHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Audio/WAudioManagerActor.h"
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
    // Устанавливаем класс GameState на наш пользовательский класс
    GameStateClass = AWTowerGameState::StaticClass();
    HUDClass = AWTowerHUD::StaticClass();


    // Настройки по умолчанию
    GameTimeLimit = 180.0f; // 3 минутs по умолчанию
    MinimumSafeHeight = -1000.0f;
    bGameEnded = false;

    // Включаем Tick для проверки условий завершения игры
    PrimaryActorTick.bCanEverTick = true;
}

void AWTowerGameMode::BeginPlay()
{
    Super::BeginPlay();
    AWAudioManagerActor* AudioManager = Cast<AWAudioManagerActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AWAudioManagerActor::StaticClass()));
    if (AudioManager)
    {
        AudioManager->PlayGameplayMusic();
    }
    UE_LOG(LogTemp, Log, TEXT("WTowerGameMode: Game started"));
    AWTowerGameState* WTowerGameState = Cast<AWTowerGameState>(GameState);
    if (WTowerGameState)
    {
        // Подписываемся на событие завершения игры
        WTowerGameState->OnGameCompleted.AddDynamic(this, &AWTowerGameMode::OnGameCompleted);
    }
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




void AWTowerGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Если игра уже завершена, пропускаем проверки
    if (bGameEnded)
    {
        return;
    }

    // Получаем GameState для проверки условий
    AWTowerGameState* WTowerGameState = Cast<AWTowerGameState>(GameState);
    if (WTowerGameState)
    {
        // Проверка времени (если задан лимит)
        float GameTime = WTowerGameState->GetGameTime();
        if (GameTimeLimit > 0.0f && GameTime >= GameTimeLimit)
        {
            // Время вышло
            EndGameWithDefeat(TEXT("Время вышло"));
            return;
        }

        // Проверка высоты игрока
        float PlayerCurrentHeight = WTowerGameState->GetPlayerCurrentHeight();
        if (PlayerCurrentHeight < MinimumSafeHeight)
        {
            // Игрок упал слишком низко
            EndGameWithDefeat(TEXT("Игрок упал"));
            return;
        }
    }
}

// Обработчик события завершения игры (вызывается из GameState)
void AWTowerGameMode::OnGameCompleted(float CompletionTime)
{
    AWAudioManagerActor* AudioManager = Cast<AWAudioManagerActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AWAudioManagerActor::StaticClass()));
    if (AudioManager)
    {
        AudioManager->StopMusic();
    }
    // Игрок собрал предмет победы или выполнил другое условие,
    // которое уже было обработано в GameState
    EndGameWithVictory();
}

// Методы для завершения игры
void AWTowerGameMode::EndGameWithVictory()
{
    // Проверяем, не завершена ли уже игра
    if (bGameEnded)
    {
        return;
    }

    bGameEnded = true;

    UE_LOG(LogTemp, Log, TEXT("WTowerGameMode: Game ended with VICTORY"));

    // Вызываем делегат для уведомления о победе
    OnGameOver.Broadcast(true, TEXT("Победа!"));

    // Здесь можно добавить код для показа экрана победы,
    // воспроизведения музыки, анимаций и т.д.
}

void AWTowerGameMode::EndGameWithDefeat(FString Reason)
{
    // Проверяем, не завершена ли уже игра
    if (bGameEnded)
    {
        return;
    }

    bGameEnded = true;

    UE_LOG(LogTemp, Log, TEXT("WTowerGameMode: Game ended with DEFEAT. Reason: %s"), *Reason);

    // Вызываем делегат для уведомления о поражении
    OnGameOver.Broadcast(false, Reason);

    // Здесь можно добавить код для показа экрана поражения,
    // воспроизведения звуков, анимаций и т.д.
}