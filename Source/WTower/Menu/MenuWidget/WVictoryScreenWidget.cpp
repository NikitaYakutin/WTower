#include "WVictoryScreenWidget.h"
#include "Components/TextBlock.h"
#include <WTower/WTowerGameInstance.h>

void UWVictoryScreenWidget::InitializeVictoryScreen(int32 FinalScore, float CompletionTime)
{
    UWTowerGameInstance* GameInstance = Cast<UWTowerGameInstance>(GetGameInstance());
    if (!GameInstance)
    {
        return;
    }
    
    // Показываем текущий результат
    if (ScoreText)
    {
        ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), FinalScore)));
    }
    
    if (TimeText)
    {
        int32 Minutes = FMath::FloorToInt(CompletionTime / 60.0f);
        int32 Seconds = FMath::FloorToInt(CompletionTime - Minutes * 60.0f);
        TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %02d:%02d"), Minutes, Seconds)));
    }
    
    // Получаем лучший счет и время из GameInstance
    int32 BestScore = GameInstance->GetBestScore();
    float BestTime = GameInstance->GetBestCompletionTime();
    
    // Отображаем лучший счет
    if (BestScoreText)
    {
        BestScoreText->SetText(FText::FromString(FString::Printf(TEXT("Best Score: %d"), BestScore)));
    }
    
    // Отображаем лучшее время
    if (BestTimeText)
    {
        if (BestTime < 999999.0f)
        {
            int32 BestMinutes = FMath::FloorToInt(BestTime / 60.0f);
            int32 BestSeconds = FMath::FloorToInt(BestTime - BestMinutes * 60.0f);
            BestTimeText->SetText(FText::FromString(FString::Printf(TEXT("Best Time: %02d:%02d"), BestMinutes, BestSeconds)));
        }
        else
        {
            BestTimeText->SetText(FText::FromString(TEXT("Best Time: --:--")));
        }
    }
    
    // Проверяем, был ли установлен новый рекорд
    bool bIsNewScoreRecord = (FinalScore >= BestScore);
    bool bIsNewTimeRecord = (CompletionTime > 0.0f && CompletionTime <= BestTime);
    
    // Показываем сообщение о новом рекорде
    if (NewRecordText)
    {
        if (bIsNewScoreRecord && bIsNewTimeRecord)
        {
            NewRecordText->SetText(FText::FromString(TEXT("NEW RECORD! Best Score and Best Time!")));
            NewRecordText->SetVisibility(ESlateVisibility::Visible);
        }
        else if (bIsNewScoreRecord)
        {
            NewRecordText->SetText(FText::FromString(TEXT("NEW RECORD! Best Score!")));
            NewRecordText->SetVisibility(ESlateVisibility::Visible);
        }
        else if (bIsNewTimeRecord)
        {
            NewRecordText->SetText(FText::FromString(TEXT("NEW RECORD! Best Time!")));
            NewRecordText->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            NewRecordText->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}