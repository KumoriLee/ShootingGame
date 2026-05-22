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
	//ボタンを離れた時の処理
	void StopMoveInput(const FInputActionValue& Value);
	//現在の目標傾斜角度
	float TargetRoll = 0.0f;

	//連射するためにtimerを作る

	// 連続発射の間隔（秒）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float FireRate = 0.15f;

	// クールダウン計算用
	float LastFireTime = 0.0f;

public:
	ARaiden();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//入力対応
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* FireAction;

	//カメラコンポーネント
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;


	//デフォルト値
	UPROPERTY(EditAnywhere)
	float Speed = 300.0f;//速度

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxRollAngle = 45.0f;//傾斜角度

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RollInterpSpeed = 5.0f;//傾斜のスムース遷移速度


	APlayerController* PlayerController;

	bool IsAlive = true;


	//メソッド
	void MoveInput(const FInputActionValue& Value);
	void FireInput();

	void HandleDestruction();
	void SetPlayerEnabled(bool Enabled);

	
};
