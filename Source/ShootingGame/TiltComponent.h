// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TiltComponent.generated.h"


/**
 * 機体傾斜（ロール回転）処理コンポーネント
 * TargetMesh の RelativeRotation.Roll を目標値へ滑らかに補間する
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHOOTINGGAME_API UTiltComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTiltComponent();

	/** 傾斜の対象となるメッシュを設定（コンストラクタで呼ぶこと） */
	void SetTargetMesh(UStaticMeshComponent* Mesh) { TargetMesh = Mesh; }

	/**
	 * 目標傾斜角度を設定（内部で ±MaxRollAngle にクランプ）
	 * @param NewTarget 目標角度（度）
	 */
	void SetTargetRoll(float NewTarget);

	/** TargetRoll を 0 にリセット（水平に戻す） */
	void ResetRoll() { SetTargetRoll(0.0f); }

	/** 傾斜が有効かどうか */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bCanRoll = true;

	/** 最大傾斜角度（度） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxRollAngle = 45.0f;

	/** 傾斜の補間速度（大きいほど速く傾く） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RollInterpSpeed = 5.0f;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	/** 回転させるメッシュへのポインタ */
	UPROPERTY()
	UStaticMeshComponent* TargetMesh = nullptr;

	/** 現在の目標傾斜角度 */
	float TargetRoll = 0.0f;
};
