// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Components/CapsuleComponent.h"

#include "Projectile.h"

#include "NiagaraFunctionLibrary.h"		// Niagara パーティクルシステムユーティリティ（エフェクト生成）

#include "BasePawn.generated.h"

UCLASS()
class SHOOTINGGAME_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();

	/**
	 * 機体傾斜の更新処理
	 * @param DeltaTime フレームのデルタ時間（秒）
	 */
	virtual void Tick(float DeltaTime) override;

	//コライダー
	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	//弾の生成点
	UPROPERTY(VisibleAnywhere)
	USceneComponent* ProjectileSpawnPoint;


	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> ProjectileClass;


	//傾斜に関する変数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bCanRoll = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxRollAngle = 45.0f;//傾斜角度

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RollInterpSpeed = 5.0f;//傾斜のスムース遷移速度

protected:
	//今の傾斜角度
	float TargetRoll = 0.0f;

	void fire();

	void HandleDestruction();

	/** 死亡時に再生する Niagara パーティクルシステム */
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* DeathParticles;

	/** 死亡時に再生するサウンド */
	UPROPERTY(EditAnywhere)
	USoundBase* BoomSound;



};
