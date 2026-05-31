#include "LevelManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"	// ゲーム静的ユーティリティ関数：OpenLevel レベル切り替え
#include "Engine/World.h"				// ワールドコンテキスト：GetWorld() で現在のレベルワールドポインタを取得

void ULevelManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	// Config 指定されたプロパティ（LevelNames）は UObject システムが .ini から自動ロードする
}

// ----------------------------------------------------------------------------
// プライベート：コア切り替えロジック
// ----------------------------------------------------------------------------

void ULevelManagerSubsystem::TransitionToLevel(FName LevelName)
{
	// ガード：空の名前は拒否
	if (LevelName.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("LevelManagerSubsystem: TransitionToLevel called with NAME_None"));
		return;
	}

	// 切り替え前のレベル名を記録（イベントブロードキャスト用）
	const FName PreviousLevel = GetCurrentLevelName();

	// 対象レベルが配列内にあればインデックスを同期
	const int32 FoundIndex = LevelNames.Find(LevelName);
	if (FoundIndex != INDEX_NONE)
	{
		CurrentLevelIndex = FoundIndex;
	}

	// イベントブロードキャスト：他システムが切り替え前にクリーンアップ/保存処理を実行できる
	OnLevelTransition.Broadcast(PreviousLevel, LevelName);

	// レベル切り替えを実行
	if (UWorld* World = GetWorld())
	{
		UGameplayStatics::OpenLevel(World, LevelName);
	}
}

// ----------------------------------------------------------------------------
// 公開 API：レベル切り替え
// ----------------------------------------------------------------------------

void ULevelManagerSubsystem::OpenLevel(FName LevelName)
{
	if (!LevelName.IsNone())
	{
		TransitionToLevel(LevelName);
	}
}

void ULevelManagerSubsystem::OpenLevelByIndex(int32 Index)
{
	if (LevelNames.IsValidIndex(Index))
	{
		TransitionToLevel(LevelNames[Index]);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LevelManagerSubsystem: OpenLevelByIndex - index %d out of range (0-%d)"), Index, LevelNames.Num() - 1);
	}
}

void ULevelManagerSubsystem::OpenNextLevel()
{
	if (LevelNames.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("LevelManagerSubsystem: OpenNextLevel called but LevelNames is empty"));
		return;
	}

	// インデックス+1、最後までいくと先頭に戻る
	int32 NextIndex = CurrentLevelIndex + 1;
	if (NextIndex >= LevelNames.Num())
	{
		NextIndex = 0;
	}
	TransitionToLevel(LevelNames[NextIndex]);
}

void ULevelManagerSubsystem::OpenPreviousLevel()
{
	if (LevelNames.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("LevelManagerSubsystem: OpenPreviousLevel called but LevelNames is empty"));
		return;
	}

	// インデックス-1、最初までいくと最後に戻る
	int32 PrevIndex = CurrentLevelIndex - 1;
	if (PrevIndex < 0)
	{
		PrevIndex = LevelNames.Num() - 1;
	}
	TransitionToLevel(LevelNames[PrevIndex]);
}

void ULevelManagerSubsystem::RestartCurrentLevel()
{
	// 現在のレベル名を優先、未初期化ならリストの最初のレベルにフォールバック
	if (const FName CurrentName = GetCurrentLevelName(); !CurrentName.IsNone())
	{
		TransitionToLevel(CurrentName);
	}
	else if (LevelNames.Num() > 0)
	{
		TransitionToLevel(LevelNames[0]);
	}
}

void ULevelManagerSubsystem::RestartGame()
{
	if (LevelNames.Num() > 0)
	{
		TransitionToLevel(LevelNames[0]);
	}
}

// ----------------------------------------------------------------------------
// 公開 API：クエリと設定
// ----------------------------------------------------------------------------

FName ULevelManagerSubsystem::GetCurrentLevelName() const
{
	if (LevelNames.IsValidIndex(CurrentLevelIndex))
	{
		return LevelNames[CurrentLevelIndex];
	}
	return NAME_None;
}

void ULevelManagerSubsystem::SetLevelNames(const TArray<FName>& InLevelNames)
{
	LevelNames = InLevelNames;
	CurrentLevelIndex = -1;		// インデックスリセット：リストが変更されたので、次回切り替え時に再設定される
}
