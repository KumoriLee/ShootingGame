// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Components/CapsuleComponent.h"

#include "Projectile.h"

#include "NiagaraFunctionLibrary.h"		// Niagara パーティクルシステム：シーン上にエフェクトを生成

#include "BasePawn.generated.h"

UCLASS()
class SHOOTINGGAME_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// この Pawn のデフォルト値を設定
	ABasePawn();

	/**
	 * 機体傾斜の更新処理
	 * @param DeltaTime フレームのデルタ時間（秒）
	 */
	virtual void Tick(float DeltaTime) override;

	/** コリジョン用カプセルコンポーネント（ルート） */
	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CapsuleComp;

	/** 機体のベースメッシュ（カプセルにアタッチ） */
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	/** 弾のスポーン位置（BaseMesh にアタッチ） */
	UPROPERTY(VisibleAnywhere)
	USceneComponent* ProjectileSpawnPoint;

	/** スポーンする弾のクラス */
	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> ProjectileClass;


	// 傾斜に関するパラメータ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bCanRoll = true;

	/** 最大傾斜角度（度） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxRollAngle = 45.0f;

	/** 傾斜の補間速度（大きいほど速く傾く） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RollInterpSpeed = 5.0f;

protected:
	/** 現在の目標傾斜角度 */
	float TargetRoll = 0.0f;

	/** 弾をスポーンして発射する */
	void fire();

	/** 死亡時の破壊処理（エフェクト・サウンド再生） */
	void HandleDestruction();

	/** 死亡時に再生する Niagara パーティクルシステム */
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* DeathParticles;

	/** 死亡時に再生するサウンド */
	UPROPERTY(EditAnywhere)
	USoundBase* BoomSound;



};
