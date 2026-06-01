// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Raiden.h"

#include "EnemyRaidenPawn.generated.h"

/**
 * 敵機（雷電タイプ）
 * AI によるプレイヤー追跡、射撃サイクル FSM、衝突ダメージ処理を担当
 */
UCLASS()
class SHOOTINGGAME_API AEnemyRaidenPawn : public ABasePawn
{
	GENERATED_BODY()
	
protected:
	// ゲーム開始時またはスポーン時に呼ばれる
	virtual void BeginPlay() override;

public:
	AEnemyRaidenPawn();

	/**
	 * 毎フレーム更新処理（移動、射撃サイクル、範囲外判定）
	 * @param DeltaTime フレームのデルタ時間（秒）
	 */
	virtual void Tick(float DeltaTime) override;

	/** 射撃を開始するプレイヤーとの距離 */
	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRange = 1500.0f;
	/** 射撃条件チェックの間隔（秒） */
	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRate = 0.5f;

	// 移動に関するパラメータ
	UPROPERTY(EditAnywhere, Category = "Movement")
	float ForwardSpeed = 300.0f;

	/** Y 軸追跡の速度 */
	UPROPERTY(EditAnywhere, Category = "Movement")
	float YTargetSpeed = 200.0f;

	/** プレイヤーを追跡し始める距離 */
	UPROPERTY(EditAnywhere, Category = "Movement")
	float SearchRange = 2000.0f;

	/** プレイヤー機へのキャッシュポインタ */
	ARaiden* Raiden;

	/** プレイヤーが射程内にいるかどうかを判定 */
	bool InFireRange();

	/** 射撃条件をチェックし、満たせば fire() を呼ぶ */
	void CheckFireCondition();
	/** 死亡時の破壊処理（エフェクト再生＋Destroy） */
	void HandleDestruction();


	/**
	 * 敵機同士または敵機とプレイヤーの衝突処理
	 * @param HitComponent 衝突した自身のコンポーネント
	 * @param OtherActor  衝突相手のアクター
	 * @param OtherComp   衝突相手のコンポーネント
	 * @param NormalImpulse 衝突の法線インパルス
	 * @param Hit         衝突詳細情報
	 */
	UFUNCTION()
	void OnEnemyHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** 画面範囲外判定コンポーネント。bAutoDestroy=false で委任により自爆処理を制御 */
	UPROPERTY(VisibleAnywhere)
	class UFrameComponent* FrameComp;

	UPROPERTY(VisibleAnywhere)
	class UHealthComponent* HealthComp;



private:
	/** 射撃サイクル FSM 用タイマー（発射中 / クールダウン中を切り替え） */
	float FireCycleTimer = 0.0f;
	/** true=発射フェーズ、false=クールダウンフェーズ */
	bool bIsFiringPhase = true;

	/** 射撃条件チェック用の反復タイマーハンドル */
	FTimerHandle FireTimerHandle;

};
