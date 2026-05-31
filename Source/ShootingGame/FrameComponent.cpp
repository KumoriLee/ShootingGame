// Fill out your copyright notice in the Description page of Project Settings.


#include "FrameComponent.h"
#include "Kismet/GameplayStatics.h"

UFrameComponent::UFrameComponent()
{
	// Tick を有効化し、毎フレーム Owner の位置をプレイヤーと比較する
	PrimaryComponentTick.bCanEverTick = true;
}

void UFrameComponent::BeginPlay()
{
	Super::BeginPlay();

	// 毎フレーム GetPlayerPawn を呼ぶオーバーヘッドを避けるため、BeginPlay で一度だけキャッシュ
	CachedPlayer = UGameplayStatics::GetPlayerPawn(this, 0);
}

void UFrameComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// プレイヤー未取得または既に範囲外判定済みの場合は何もしない
	if (!CachedPlayer || bIsOutOfBounds)
	{
		return;
	}

	const float PlayerX = CachedPlayer->GetActorLocation().X;
	const float OwnerX = GetOwner()->GetActorLocation().X;

	// Owner の X 座標がプレイヤーより一定距離以上進んだら範囲外と判定
	if (OwnerX > PlayerX + OutOfBoundsDistance)
	{
		bIsOutOfBounds = true;

		// 範囲外委任をブロードキャスト。Enemy など bAutoDestroy=false の Owner が自前処理を行う
		OnOutOfBounds.Broadcast();

		// 自動破棄が有効なら、遅延秒数後に Owner を破棄
		if (bAutoDestroy && SelfDestructDelay > 0.0f)
		{
			GetOwner()->SetLifeSpan(SelfDestructDelay);
		}
	}
}
