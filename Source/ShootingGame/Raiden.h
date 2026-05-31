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


/**
 * 
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

	//連射するためにtimerを作る

	// 連続発射の間隔（秒）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float FireRate = 0.15f;

	// クールダウン計算用
	float LastFireTime = 0.0f;

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

	//入力対応
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* RestartAction;

	//カメラコンポーネント
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;


	//デフォルト値
	UPROPERTY(EditAnywhere)
	float Speed = 300.0f;//速度

	APlayerController* PlayerController;

	bool IsAlive = true;
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

	UPROPERTY(VisibleAnywhere)
	class UHealthComponent* HealthComp;
};
