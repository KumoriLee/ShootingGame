// Fill out your copyright notice in the Description page of Project Settings.


#include "Raiden.h"

#include "InputMappingContext.h"
//カメラのヘーダファイル
#include "Camera/CameraComponent.h"


//UGameplayStatics
#include "Kismet/GameplayStatics.h"
#include "ShootingGameMode.h"




ARaiden::ARaiden()
{
	SpringArmComp = CreateDefaultSubobject <USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(CapsuleComp);

	CameraComp = CreateDefaultSubobject <UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

}

void ARaiden::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(Controller);
	//コントローラー初期化
	if (PlayerController)//PlayerControllerを取得
	{
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())//LocalPlayerを取得
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
			{
				//このマッピングコンテキストで定義されたすべての入力アクションをエンジンが認識し、処理できるように
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}
void ARaiden::StopMoveInput(const FInputActionValue& Value)
{
	TargetRoll = 0.0f;
}

void ARaiden::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//CapsuleComponent を取得し、そのカプセル状のデバッグ形状を描画する
	if (CapsuleComp)//存在する場合
	{
		FVector CapsuleLocation = CapsuleComp->GetComponentLocation();//座標のベクトルを獲得
		float	CapsuleRadius = CapsuleComp->GetScaledCapsuleRadius();//半径
		float	CapsuleHalfHeight = CapsuleComp->GetScaledCapsuleHalfHeight();//直径

		//デバッグ用のcapsuleを描画する
		DrawDebugCapsule(GetWorld(), CapsuleLocation, CapsuleHalfHeight, CapsuleRadius, FQuat::Identity, FColor::Green, false, -1.0f, 0, 2.0f);
	}
	
	
	
}

void ARaiden::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARaiden::MoveInput);
		//キー解放または中断時にリターン（戻る）処理を実行
		EIC->BindAction(MoveAction, ETriggerEvent::Completed, this, &ARaiden::StopMoveInput);
		EIC->BindAction(MoveAction, ETriggerEvent::Canceled, this, &ARaiden::StopMoveInput);

		EIC->BindAction(FireAction, ETriggerEvent::Triggered, this, &ARaiden::FireInput);
		EIC->BindAction(RestartAction, ETriggerEvent::Triggered, this, &ARaiden::OnRestartInput);
	}
}

void ARaiden::MoveInput(const FInputActionValue& Value)
{
	if (!bCanAct || !IsAlive) return;

	//入力システムから Vector2D の値を取得
	FVector2D MovementVector = Value.Get<FVector2D>();
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());

	// 入力がある場合のみ移動処理を行う
	if (MovementVector.SizeSquared() > 0.0f)
	{
		// 入力値をゲーム内の移動方向（X軸・Y軸）にマッピング
		FVector DeltaLocation = FVector::ZeroVector;

		DeltaLocation.X = MovementVector.Y * Speed * DeltaTime; // 前後の移動量
		DeltaLocation.Y = MovementVector.X * Speed * DeltaTime; // 左右の移動量

		// 移動量を一度にまとめて適用
		// 第2引数を true にすることで、壁などの衝突判定を有効に
		AddActorLocalOffset(DeltaLocation, true);

		// 右入力(1.0)なら45度、左入力(-1.0)なら-45度に傾斜
		TargetRoll = MovementVector.X * MaxRollAngle;
	}
}

void ARaiden::FireInput()
{
	if (!bCanAct || !IsAlive) return;

	//現在のゲーム内の絶対時間を取得（秒）
	float CurrentTime = GetWorld()->GetTimeSeconds();

	//現在の時間 - 前回撃った時間 が FireRate以上経過しているか判定
	if (CurrentTime - LastFireTime >= FireRate)
	{
		fire();

		//クールダウンをリセットする
		LastFireTime = CurrentTime;
	}
}

void ARaiden::HandleDestruction()
{
	Super::HandleDestruction();

	IsAlive = false;
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
}

void ARaiden::SetPlayerEnabled(bool Enabled)
{
	bCanAct = Enabled;

	if (PlayerController)
	{
		if (Enabled)
		{
			EnableInput(PlayerController);
		}
		else
		{
			DisableInput(PlayerController);
			TargetRoll = 0.0f;
		}
	}
}

void ARaiden::OnRestartInput(const FInputActionValue& Value)
{
	AShootingGameMode* GM = Cast<AShootingGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM && GM->bGameEnded)
	{
		GM->RestartGame();
	}
}
