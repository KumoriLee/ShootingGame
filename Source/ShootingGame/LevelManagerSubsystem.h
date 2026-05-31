#pragma once

#include "CoreMinimal.h"					// UEコア型
#include "Subsystems/GameInstanceSubsystem.h"		// GameInstanceサブシステム基底クラス：GameInstanceライフサイクルに合わせて自動生成/破棄
#include "LevelManagerSubsystem.generated.h"

/**
 * レベル切り替え前にブロードキャストするイベント
 * @param FromLevel	現在のレベル名（初回切り替え時は NAME_None の可能性あり）
 * @param ToLevel	開こうとしているレベル名
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnLevelTransition,
	FName, FromLevel,
	FName, ToLevel
);

/**
 * レベル管理サブシステム（UGameInstanceSubsystem）
 *
 * データ駆動型のレベルマネージャー。旧BattleBlasterGameInstanceのハードコードされたLevel_%d命名方式を置き換え。
 * LevelNames配列でレベル一覧を設定可能。名前・インデックス・順次切り替えに対応し、レベル名の命名規則を強制しない。
 *
 * 特徴：
 * - 特定のGameInstanceサブクラスに依存せず、どのプロジェクトにコピーしても使用可能
 * - 3つの設定方法：DefaultGame.ini / Blueprint SetLevelNames() / サブシステムClass Defaults
 * - 公開APIはすべてBlueprintCallable、ブループリントから直接呼び出し可能
 * - OpenLevelは配列に登録されていないレベルもロード可能（一時的な遷移）
 *
 * 使用例（C++）：
 * @code
 * ULevelManagerSubsystem* LM = GetGameInstance()->GetSubsystem<ULevelManagerSubsystem>();
 * LM->OpenLevel("Desert_Fortress");
 * @endcode
 *
 * 使用例（Blueprint）：
 *   Get Game Instance → Get Subsystem (LevelManagerSubsystem) → Open Next Level
 */
UCLASS(Config = Game)
class SHOOTINGGAME_API ULevelManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/**
	 * レベル名を指定して任意のレベルを開く（配列外のレベルもロード可能）
	 * @param LevelName レベルアセット名。Content/Maps/ 以下の .umap ファイル名と一致（パスプレフィックス不要）
	 */
	UFUNCTION(BlueprintCallable, Category = "Level Manager")
	void OpenLevel(FName LevelName);

	/**
	 * 配列のインデックスでレベルを開く
	 * @param Index 0ベースのインデックス、範囲 [0, LevelNames.Num()-1]
	 */
	UFUNCTION(BlueprintCallable, Category = "Level Manager")
	void OpenLevelByIndex(int32 Index);

	/** 次のレベルを開く（最後までいくと先頭に戻る） */
	UFUNCTION(BlueprintCallable, Category = "Level Manager")
	void OpenNextLevel();

	/** 前のレベルを開く（最初までいくと最後に戻る） */
	UFUNCTION(BlueprintCallable, Category = "Level Manager")
	void OpenPreviousLevel();

	/**
	 * 現在のレベルをリロードする
	 * 優先順位：現在のレベル名 → LevelNames[0]（フォールバック）
	 */
	UFUNCTION(BlueprintCallable, Category = "Level Manager")
	void RestartCurrentLevel();

	/** 最初のレベル（LevelNames[0]）に戻って最初からやり直す */
	UFUNCTION(BlueprintCallable, Category = "Level Manager")
	void RestartGame();

	/**
	 * 実行時にレベル一覧をすべて置き換える（同時に現在のインデックスを -1 にリセット）
	 * @param InLevelNames 新しいレベル名の配列
	 */
	UFUNCTION(BlueprintCallable, Category = "Level Manager")
	void SetLevelNames(const TArray<FName>& InLevelNames);

	/** @return LevelNames内の現在のレベルインデックス（0ベース、-1は未初期化） */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Level Manager")
	int32 GetCurrentLevelIndex() const { return CurrentLevelIndex; }

	/** @return 現在のレベル名（未初期化時は NAME_None） */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Level Manager")
	FName GetCurrentLevelName() const;

	/** @return レベル一覧の総数 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Level Manager")
	int32 GetLevelCount() const { return LevelNames.Num(); }

	/** レベル切り替えイベント：TransitionToLevel内、OpenLevelの前にブロードキャスト */
	UPROPERTY(BlueprintAssignable, Category = "Level Manager")
	FOnLevelTransition OnLevelTransition;

	/**
	 * レベル名一覧 —— 切り替え操作の基準
	 *
	 * 設定方法（優先順）：
	 * 1. DefaultGame.ini：[/Script/ShootingGame.LevelManagerSubsystem] LevelNames=(...) LevelNames=(...)
	 * 2. Blueprint：SetLevelNames() を呼び出す
	 * 3. Class Defaults：エディタのプロジェクト設定で直接編集
	 *
	 * デフォルト値は従来の Level_1/2/3 形式を維持。移行後も追加設定なしで動作する。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "Levels")
	TArray<FName> LevelNames = { TEXT("Level_1"), TEXT("Level_2"), TEXT("Level_3") };

private:
	/**
	 * 実際のレベル切り替えを実行
	 * 1. CurrentLevelIndex を更新（LevelName が配列内にある場合）
	 * 2. OnLevelTransition をブロードキャスト
	 * 3. UGameplayStatics::OpenLevel を呼び出す
	 */
	void TransitionToLevel(FName LevelName);

	/** LevelNames内の現在のレベルインデックス（0ベース、-1は未初期化） */
	UPROPERTY()
	int32 CurrentLevelIndex = -1;
};
