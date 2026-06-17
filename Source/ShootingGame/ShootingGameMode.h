// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "BasePawn.h"
#include "Raiden.h"
#include "EnemyRaidenPawn.h"
#include "ScreenMessage.h"
#include "ShootingGameMode.generated.h"

/**
 * 生成エントリ構造体
 * 各エントリが「何を・どこに・どれくらいの間隔で」生成するかを定義する
 */
USTRUCT(BlueprintType)
struct FSpawnEntry
{
	GENERATED_BODY()

	/** 生成するアクターのクラス（ABasePawn 派生であれば何でも可） */
	UPROPERTY(EditAnywhere, Category = "Spawn")
	TSubclassOf<ABasePawn> ActorClass;

	/** 生成間隔（秒） */
	UPROPERTY(EditAnywhere, Category = "Spawn")
	float SpawnInterval = 3.0f;

	/** プレイヤーからの相対生成位置 */
	UPROPERTY(EditAnywhere, Category = "Spawn")
	FVector SpawnOffset = FVector(-4000.0f, 0.0f, 0.0f);

	/** Y方向ランダム範囲（SpawnOffset.Y ± Range の範囲でランダム） */
	UPROPERTY(EditAnywhere, Category = "Spawn")
	float SpawnRangeY = 2000.0f;

	/** 有効/無効 */
	UPROPERTY(EditAnywhere, Category = "Spawn")
	bool bEnabled = true;

	/** 前回生成からの経過時間（内部カウンタ、エディタ非表示） */
	float ElapsedSinceLastSpawn = 0.0f;
};

/**
 * シューティングゲームのゲームモード
 * カウントダウン → ゲーム進行 → 敵スポーン → 勝利/敗北 → リスタート のフローを管理
 */
UCLASS()
class SHOOTINGGAME_API AShootingGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	// ゲーム開始時またはスポーン時に呼ばれる
	virtual void BeginPlay() override;

public:
	/** プレイヤー機へのキャッシュポインタ */
	ARaiden* raiden;
	/** 現在のスコア */
	int32 point = 0;

	/** 生成エントリのリスト。エディタで敵・障害物などを自由に追加可能 */
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TArray<FSpawnEntry> SpawnEntries;

	// ==================== カウントダウン関連パラメータ ====================
	UPROPERTY(EditAnywhere, Category = "Game Rules")
	float StartDelay = 3.0f; // ゲーム開始前のカウントダウン（秒）

	UPROPERTY(EditAnywhere, Category = "Game Rules")
	float GameDuration = 30.0f; // ゲームの制限時間（秒）

	// ================================================================

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UScreenMessage> ScreenMessageClass;

	/**
	 * アクターが死亡した際の処理
	 * @param DeadActor 死亡したアクター（プレイヤーまたは敵）
	 */
	void ActorDied(AActor* DeadActor);

	// UMG（UI）ブループリントがカウントダウン残り時間を取得するための関数
	UFUNCTION(BlueprintPure, Category = "Game Rules")
	float GetStartDelayRemaining() const;

	UFUNCTION(BlueprintPure, Category = "Game Rules")
	float GetGameTimeRemaining() const;

	bool bGameEnded = false;

	void RestartGame();  // 現在のレベルを再読み込みしてリスタート

private:
	FTimerHandle SpawnTimerHandle;
	FTimerHandle GameTimerHandle;             // 30秒制限時間のタイマーハンドル
	FTimerHandle CountdownTimerHandle;        // カウントダウン表示更新（1秒ごと）
	FTimerHandle UITimerHandle;               // ゲーム中 Time 表示更新

	int32 CountdownStep;          // カウントダウン現在値（3→2→1→0でGo）

	UScreenMessage* ScreenMessageWidget;

	/**
	 * 統一生成タイマーコールバック。全 SpawnEntries を巡回し、間隔に達したものを生成
	 */
	void SpawnTick();

	/**
	 * 単一エントリからアクターを生成
	 */
	void DoSpawn(const FSpawnEntry& Entry);

	// ゲーム状態制御関数
	void StartGame();    // カウントダウン終了、ゲーム開始
	void EndGame();      // 制限時間到達、ゲーム終了

	/**
	 * シーン内の全 BasePawn 派生アクターの Tick を停止する
	 * ゲーム終了時またはプレイヤー死亡時に呼び出される
	 */
	void StopEnemy();
};
