// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"


#include "Rocks.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAME_API ARocks : public ABasePawn
{
	GENERATED_BODY()


public:
	ARocks();

	virtual void Tick(float DeltaTime) override;

	// 移動に関するパラメータ
	UPROPERTY(EditAnywhere, Category = "Movement")
	float ForwardSpeed = 300.0f;


	/** 死亡時の破壊処理（エフェクト再生＋Destroy） */
	void HandleDestruction();

	/** 画面範囲外判定コンポーネント。bAutoDestroy=false で委任により自爆処理を制御 */
	UPROPERTY(VisibleAnywhere)
	class UFrameComponent* FrameComp;

	UPROPERTY(VisibleAnywhere)
	class UHealthComponent* HealthComp;

	/** 敵機衝突ダメージ処理コンポーネント */
	UPROPERTY(VisibleAnywhere)
	class UEnemyHitComponent* EnemyHitComp;

	
};
