// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "NiagaraComponent.h"			// Niagara コンポーネント：アクターにアタッチするパーティクルシステム
#include "NiagaraFunctionLibrary.h"		// Niagara 関数ライブラリ：シーン上にパーティクルエフェクトを生成


#include "Projectile.generated.h"

UCLASS()
class SHOOTINGGAME_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	/**
	 * 毎フレーム更新処理（画面外判定と自動破棄）
	 * @param DeltaTime フレームのデルタ時間（秒）
	 */
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovementComp;

	/** 飛行軌跡の尾引きパーティクル（弾にアタッチして持続再生） */
	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* TrialParticles;

	/** 命中時に生成する爆発パーティクルシステム */
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* HitParticles;

	/** 発射音 */
	UPROPERTY(EditAnywhere)
	USoundBase* LaunchSound;

	/** 命中音 */
	UPROPERTY(EditAnywhere)
	USoundBase* HitSound;


	UPROPERTY(EditAnywhere)
	float Damage = 25.0f;

	/**
	 * 弾の衝突処理
	 * @param HitComponent 衝突した自身のメッシュコンポーネント
	 * @param OtherActor   衝突相手のアクター
	 * @param OtherComp    衝突相手のコンポーネント
	 * @param NormalImpulse 衝突の法線インパルス
	 * @param Hit          衝突詳細情報
	 */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	APawn* CachedPlayerPawn;
	bool bIsOutOfBounds = false;
};
