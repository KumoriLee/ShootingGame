// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyHitComponent.generated.h"

/**
 * 敵機の衝突ダメージ処理コンポーネント
 * オーナーの CapsuleComp に OnComponentHit をバインドし、プレイヤーとの衝突時に相互ダメージを与える
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHOOTINGGAME_API UEnemyHitComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEnemyHitComponent();

protected:
	virtual void BeginPlay() override;

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
};
