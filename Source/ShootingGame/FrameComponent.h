// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FrameComponent.generated.h"

/** 範囲外判定時にブロードキャストされる委任。Owner がバインドして独自の後処理を行える */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOutOfBounds);

/**
 * 画面範囲外判定コンポーネント
 * Owner がプレイヤーの X 座標より一定距離以上離れたら範囲外と判定し、委任をブロードキャストする
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHOOTINGGAME_API UFrameComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFrameComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** 範囲外と判定するプレイヤーからの X 軸距離 */
	UPROPERTY(EditAnywhere, Category = "FrameCheck")
	float OutOfBoundsDistance = 500.0f;

	/** 範囲外判定後の遅延破棄時間（秒） */
	UPROPERTY(EditAnywhere, Category = "FrameCheck")
	float SelfDestructDelay = 3.0f;

	/** true の場合、範囲外判定時に自動で SetLifeSpan を呼ぶ */
	UPROPERTY(EditAnywhere, Category = "FrameCheck")
	bool bAutoDestroy = true;

	/** 範囲外になった時にブロードキャストされる委任 */
	UPROPERTY(BlueprintAssignable, Category = "FrameCheck")
	FOnOutOfBounds OnOutOfBounds;

	/** 範囲外フラグを返す */
	UFUNCTION(BlueprintCallable, Category = "FrameCheck")
	bool IsOutOfBounds() const { return bIsOutOfBounds; }

private:
	/** キャッシュされたプレイヤー Pawn */
	APawn* CachedPlayer;

	/** 範囲外判定済みフラグ（再判定防止） */
	bool bIsOutOfBounds = false;
};
