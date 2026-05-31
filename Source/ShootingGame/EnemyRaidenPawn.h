// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Raiden.h"

#include "EnemyRaidenPawn.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAME_API AEnemyRaidenPawn : public ABasePawn
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	AEnemyRaidenPawn();

	/**
	 * 毎フレーム更新処理（移動、射撃サイクル、範囲外判定）
	 * @param DeltaTime フレームのデルタ時間（秒）
	 */
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRange = 1500.0f; 
	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRate = 0.5f; 

	//動きに関する変数
	UPROPERTY(EditAnywhere, Category = "Movement")
	float ForwardSpeed = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float YTargetSpeed = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float SearchRange = 2000.0f;


	ARaiden* Raiden;

	bool InFireRange();

	void CheckFireCondition();
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


private:
	float FireCycleTimer = 0.0f;
	bool bIsFiringPhase = true;

	FTimerHandle FireTimerHandle;

};
