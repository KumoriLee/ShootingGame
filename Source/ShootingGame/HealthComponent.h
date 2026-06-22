// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShootingGameMode.h"

#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTINGGAME_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// このコンポーネントのデフォルト値を設定
	UHealthComponent();

protected:
	// ゲーム開始時に呼ばれる
	virtual void BeginPlay() override;

public:
	

	/** 最大 HP */
	UPROPERTY(EditAnywhere)
	float maxHealth = 100.0f;

	/** 現在の HP */
	UPROPERTY(VisibleAnywhere)
	float health;

	/** GameMode へのキャッシュポインタ（ActorDied 通知用） */
	AShootingGameMode* ShootingGameMode;

	/**
	 * ダメージ通知を受け取り、HPを減算する
	 * @param DamagedActor  ダメージを受けたアクター
	 * @param Damage        ダメージ量
	 * @param DamageType    ダメージの種類
	 * @param InstigatedBy  ダメージを引き起こしたコントローラー
	 * @param DamageCauser  ダメージを引き起こしたアクター
	 */
	UFUNCTION()
	void OnDamageTaken(
		AActor* DamagedActor,
		float Damage,
		const class UDamageType* DamageType,
		class AController* InstigatedBy,
		AActor* DamageCauser);
};
