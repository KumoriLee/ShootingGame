// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"

#include "GameFrameWork/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"//build.csにも加入

#include "InputAction.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"


#include "Raiden.generated.h"
//入力対応
class UInputMappingContext;
//カメラ対応
class UCameraComponent;
//マップ対応
class ABackground;


/**
 * プレイヤー機（雷電）
 * EnhancedInput による移動・射撃・リスタート操作、SpringArm カメラ制御を担当
 */
UCLASS()
class SHOOTINGGAME_API ARaiden : public ABasePawn
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	/**
	 * ボタン解放時の移動停止処理
	 * @param Value 入力アクション値（バインドシグネチャ準拠のため受け取るのみ）
	 */
	void StopMoveInput(const FInputActionValue& Value);

	// 連射制御用タイマー

	/** 連続発射の間隔（秒） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float FireRate = 0.15f;

	/** 最終発射時刻（クールダウン計算用） */
	float LastFireTime = 0.0f;

	// カメラの移動可能範囲：左端（Y 軸最小値）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	float CameraMinY = -1500.0f;

	// カメラの移動可能範囲：右端（Y 軸最大値）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	float CameraMaxY = 1500.0f;

	/** プレイヤー移動可能範囲：左端 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bounds Settings|Player")
	float PlayerMinY = -1500.0f;
	/** プレイヤー移動可能範囲：右端 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bounds Settings|Player")
	float PlayerMaxY = 1500.0f;

	/** 初期カメラオフセット（BeginPlay で記録） */
	FVector InitialCameraOffset;

	UPROPERTY(VisibleAnywhere)
	ABackground* MapBackground;

public:
	ARaiden();

	/**
	 * 毎フレーム更新処理（デバッグ用カプセル描画）
	 * @param DeltaTime フレームのデルタ時間（秒）
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * 入力コンポーネントをセットアップし、アクションとのバインドを行う
	 * @param PlayerInputComponent このPawnに割り当てられた入力コンポーネント
	 */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 入力マッピングコンテキスト
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	/** 移動入力アクション */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	/** 射撃入力アクション */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* FireAction;

	/** リスタート入力アクション */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* RestartAction;

	/** 終了入力アクション */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* QuitAction;

	// カメラコンポーネント
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	/** 移動速度 */
	UPROPERTY(EditAnywhere)
	float Speed = 300.0f;

	/** プレイヤーコントローラーへのキャッシュポインタ */
	APlayerController* PlayerController;

	/** 生存フラグ */
	bool IsAlive = true;
	/** 操作受付フラグ（カウントダウン中・ゲーム終了後は false） */
	bool bCanAct = true;


	/**
	 * 入力方向に基づいてプレイヤーを移動させる
	 * @param Value EnhancedInput からの2D入力値（X:左右, Y:前後）
	 */
	void MoveInput(const FInputActionValue& Value);
	void FireInput();

	void HandleDestruction();
	/**
	 * プレイヤーの入力受付を有効/無効に切り替える
	 * @param Enabled true で入力を有効化、false で無効化（TargetRoll もリセット）
	 */
	void SetPlayerEnabled(bool Enabled);
	void OnRestartInput(const FInputActionValue& Value);
	void OnQuitInput(const FInputActionValue& Value);

	/** 体力コンポーネント */
	UPROPERTY(VisibleAnywhere)
	class UHealthComponent* HealthComp;
};
