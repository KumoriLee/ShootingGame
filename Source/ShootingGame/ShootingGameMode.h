// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "Raiden.h"
#include "EnemyRaidenPawn.h"
#include "ScreenMessage.h"
#include "ShootingGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAME_API AShootingGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ARaiden* raiden;
	int32 enemyCount;
	int32 point = 0;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AEnemyRaidenPawn> EnemyClassToSpawn;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	float SpawnInterval = 3.0f;

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
	 * 敵を生成する
	 * プレイヤーの前方 X=-4000 の範囲内、Y軸はランダムな位置にスポーンさせる
	 */
	void SpawnEnemy();

	// ゲーム状態制御関数
	void StartGame();    // カウントダウン終了、ゲーム開始
	void EndGame();      // 制限時間到達、ゲーム終了

	/**
	 * シーン内の全敵機のTickを停止する
	 * ゲーム終了時またはプレイヤー死亡時に呼び出される
	 */
	void StopEnemy();
};
