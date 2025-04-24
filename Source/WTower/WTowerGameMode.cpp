#include "WTowerGameMode.h"
#include "PlayerCharacter.h"
#include "WTowerPlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "WTowerGameState.h"
#include "WTowerHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Audio/WAudioManagerActor.h"
#include "WTowerGameInstance.h"
#include "Menu/MenuGameMode.h"
#include "Menu/MenuPlayerController.h"
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

    // Инициализация аудио менеджера
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

    // Создаем и инициализируем UIManager
    if (UIManagerClass)
    {
        UIManager = NewObject<UWUIManager>(this, UIManagerClass);
    }
    else
    {
        UIManager = NewObject<UWUIManager>(this);
    }

    // Получаем контроллер игрока
    APlayerController* DefaultPC = GetWorld()->GetFirstPlayerController();
    if (DefaultPC)
    {
        // Инициализируем UIManager с контроллером
        UIManager->Initialize(DefaultPC);

        // Устанавливаем классы виджетов
        UIManager->SetWidgetClasses(
            MainMenuWidgetClass,
            PauseMenuWidgetClass,
            SettingsMenuWidgetClass,
            VictoryScreenWidgetClass,
            DefeatMenuWidgetClass,
            HUDWidgetClass
        );


            // В игровом уровне показываем HUD
            UIManager->ShowHUD();

            // Устанавливаем режим ввода для игры
            FInputModeGameOnly InputMode;
            DefaultPC->SetInputMode(InputMode);
            DefaultPC->SetShowMouseCursor(false);
        

        // Устанавливаем UIManager в контроллере
        AWTowerPlayerController* PC = Cast<AWTowerPlayerController>(DefaultPC);
        if (PC)
        {
            PC->SetUIManager(UIManager);
        }


    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("WTowerGameMode: No player controller available!"));
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
    // Check if game is already ended
    if (bGameEnded)
    {
        return;
    }

    bGameEnded = true;
    UGameplayStatics::SetGamePaused(this, true);
    UpdateBestScoreAndTime();
    UE_LOG(LogTemp, Log, TEXT("WTowerGameMode: Game ended with VICTORY"));

    // Show victory screen
    AWTowerPlayerController* PC = Cast<AWTowerPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
    if (PC && PC->GetUIManager())
    {
        AWTowerGameState* WTowerGameState = Cast<AWTowerGameState>(GameState);
        if (WTowerGameState)
        {
            PC->GetUIManager()->ShowVictoryScreen(
                WTowerGameState->GetScore(), 
                WTowerGameState->GetCompletionTime()
            );
        }
    }

    // Broadcast delegate for victory
    OnGameOver.Broadcast(true, TEXT("Победа!"));
}

void AWTowerGameMode::EndGameWithDefeat(FString Reason)
{
    // Check if game is already ended
    if (bGameEnded)
    {
        return;
    }

    bGameEnded = true;
    UGameplayStatics::SetGamePaused(this, true);
    UpdateBestScoreAndTime();
    UE_LOG(LogTemp, Log, TEXT("WTowerGameMode: Game ended with DEFEAT. Reason: %s"), *Reason);

    // Show defeat screen
    AWTowerPlayerController* PC = Cast<AWTowerPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
    if (PC && PC->GetUIManager())
    {
        PC->GetUIManager()->ShowDefeatScreen(Reason);
    }

    // Broadcast delegate for defeat
    OnGameOver.Broadcast(false, Reason);

}
void AWTowerGameMode::UpdateBestScoreAndTime()
{
    // Get game instance
    UWTowerGameInstance* GameInstance = Cast<UWTowerGameInstance>(GetGameInstance());
    if (!GameInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get WTowerGameInstance in GameMode!"));
        return;
    }

    // Get game state
    AWTowerGameState* WTowerGameState = Cast<AWTowerGameState>(GameState);
    if (!WTowerGameState)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get WTowerGameState in GameMode!"));
        return;
    }

    // Update best score
    int32 CurrentScore = WTowerGameState->GetScore();
    bool bNewBestScore = GameInstance->UpdateBestScore(CurrentScore);

    if (bNewBestScore)
    {
        UE_LOG(LogTemp, Log, TEXT("New best score achieved: %d"), CurrentScore);
    }

    // If game was completed, update best time
    if (WTowerGameState->IsGameCompleted())
    {
        float CompletionTime = WTowerGameState->GetCompletionTime();
        bool bNewBestTime = GameInstance->UpdateBestCompletionTime(CompletionTime);

        if (bNewBestTime)
        {
            int32 Minutes = FMath::FloorToInt(CompletionTime / 60.0f);
            int32 Seconds = FMath::FloorToInt(CompletionTime - Minutes * 60.0f);
            UE_LOG(LogTemp, Log, TEXT("New best completion time achieved: %02d:%02d"), Minutes, Seconds);
        }
    }

    // Save game data to ensure everything is persisted
    GameInstance->SaveGameData();
}

// Добавьте следующие методы в конец файла:

void AWTowerGameMode::ShowMenu(EWMenuType MenuType)
{
    if (UIManager)
    {
        UIManager->ShowMenu(MenuType);
    }
}

void AWTowerGameMode::CloseCurrentMenu()
{
    if (UIManager)
    {
        UIManager->CloseCurrentMenu();
    }
}

void AWTowerGameMode::ReturnToPreviousMenu()
{
    if (UIManager)
    {
        UIManager->ReturnToPreviousMenu();
    }
}