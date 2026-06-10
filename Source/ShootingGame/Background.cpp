// Fill out your copyright notice in the Description page of Project Settings.


#include "Background.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABackground::ABackground()
{
	PrimaryActorTick.bCanEverTick = false;

	// メッシュを生成してルートコンポーネントに設定
	BackgroundMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackgroundMesh"));
	RootComponent = BackgroundMesh;

	// 【物理・レンダリング最適化】背景板はコリジョン不要、影不要、デカール不要
	BackgroundMesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	BackgroundMesh->SetCastShadow(false);
	BackgroundMesh->bReceivesDecals = false;

	// 内部変数の初期化
	CurrentSpeed = FLT_MAX;  // 哨兵値：BeginPlayのSetScrollSpeedが必ず実行されるようにする
	BaseOffset = 0.0f;
	LastSpeedChangeTime = 0.0f;
}

// Called when the game starts or when spawned
void ABackground::BeginPlay()
{
	Super::BeginPlay();
	
	// モデルにマテリアルがアサインされているか確認
	if (BackgroundMesh->GetMaterial(0) != nullptr)
	{
		// 動的マテリアルインスタンスを作成
		DynamicMat = BackgroundMesh->CreateAndSetMaterialInstanceDynamic(0);

		// ゲーム開始時のワールド時間をマテリアルに同期
		LastSpeedChangeTime = GetWorld()->GetTimeSeconds();
		SetScrollSpeed(InitialScrollSpeed);
	}
}

void ABackground::SetScrollSpeed(float NewSpeed)
{
	// マテリアル未準備、または要求速度が現在速度と同じなら計算をスキップ
	if (!DynamicMat || FMath::IsNearlyEqual(CurrentSpeed, NewSpeed))
	{
		return;
	}

	// 現在のワールド絶対時間を取得（ゲーム一時停止に対応した時間システム）
	float CurrentGameTime = GetWorld()->GetTimeSeconds();

	// 前回の速度変更から現在までに蓄積された UV オフセットを清算
	BaseOffset += (CurrentGameTime - LastSpeedChangeTime) * CurrentSpeed;

	// 【ジッター防止】小数部のみを保持する。
	// テクスチャは Wrapのため、オフセット 100.2 と 0.2 は見た目が完全に一致する。
	// しかし長時間プレイで値が大きくなると、浮動小数点精度の低下により画面が激しく震える。
	BaseOffset = FMath::Fmod(BaseOffset, 1.0f);
	if (BaseOffset < 0.0f)
	{
		BaseOffset += 1.0f; // 値を常に 0.0 〜 1.0 の範囲に正規化
	}

	// 状態更新：新しい速度と速度変更時刻を保存
	CurrentSpeed = NewSpeed;
	LastSpeedChangeTime = CurrentGameTime;

	// 計算したベースオフセット・開始時刻・速度をマテリアルに送信（以降のスクロール計算は GPU が担当）
	DynamicMat->SetScalarParameterValue(FName("BaseOffset"), BaseOffset);
	DynamicMat->SetScalarParameterValue(FName("StartTime"), LastSpeedChangeTime);
	DynamicMat->SetScalarParameterValue(FName("Speed"), CurrentSpeed);
}



