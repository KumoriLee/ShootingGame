// Fill out your copyright notice in the Description page of Project Settings.


#include "Raiden.h"

#include "InputMappingContext.h"
//カメラのヘーダファイル
#include "Camera/CameraComponent.h"


//UGameplayStatics
#include "Kismet/GameplayStatics.h"
#include "ShootingGameMode.h"
#include "HealthComponent.h"





ARaiden::ARaiden()
{
	SpringArmComp = CreateDefaultSubobject <USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(CapsuleComp);

	CameraComp = CreateDefaultSubobject <UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HtalthComp"));


}

void ARaiden::BeginPlay()
{
	Super::BeginPlay();

	if (SpringArmComp)
	{
		InitialCameraOffset = SpringArmComp->GetRelativeLocation();

		// 絶対位置に設定（位置はルートコンポーネントに追従せず、回転とスケールは追従）
		SpringArmComp->SetAbsolute(true, false, false);
	}

	PlayerController = Cast<APlayerController>(Controller);
	// コントローラー初期化 & EnhancedInput サブシステムのセットアップ
	if (PlayerController)
	{
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
			{
				// このマッピングコンテキストで定義されたすべての入力アクションをエンジンに登録
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
	// デバッグ用：CapsuleComponent の形状を視覚化
	if (CapsuleComp)
	{
		FVector CapsuleLocation = CapsuleComp->GetComponentLocation();
		float CapsuleRadius = CapsuleComp->GetScaledCapsuleRadius();
		float CapsuleHalfHeight = CapsuleComp->GetScaledCapsuleHalfHeight();

		// 緑色のデバッグカプセルを描画
		DrawDebugCapsule(GetWorld(), CapsuleLocation, CapsuleHalfHeight, CapsuleRadius, FQuat::Identity, FColor::Green, false, -1.0f, 0, 2.0f);
	}
	
	FVector PlayerLoc = GetActorLocation();
	PlayerLoc.Y = FMath::Clamp(PlayerLoc.Y, PlayerMinY, PlayerMaxY);
	SetActorLocation(PlayerLoc);

	if (SpringArmComp)
	{
		FVector CameraLoc = PlayerLoc; // カメラは本来プレイヤー位置に追従する
		CameraLoc.Y = FMath::Clamp(CameraLoc.Y, CameraMinY, CameraMaxY);

		// カメラ位置を適用（ブループリントで設定済みの高さオフセットを加味）
		SpringArmComp->SetWorldLocation(CameraLoc - InitialCameraOffset);
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

		EIC->BindAction(QuitAction, ETriggerEvent::Triggered, this, &ARaiden::OnQuitInput);
	}
}

void ARaiden::MoveInput(const FInputActionValue& Value)
{
	if (!bCanAct || !IsAlive) return;

	// EnhancedInput から Vector2D の値を取得
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

		// 右入力(1.0)なら MaxRollAngle 度、左入力(-1.0)なら -MaxRollAngle 度に傾斜
		TargetRoll = MovementVector.X * MaxRollAngle;
	}
}

void ARaiden::FireInput()
{
	if (!bCanAct || !IsAlive) return;

	// 現在のゲーム内絶対時間を取得（秒）
	float CurrentTime = GetWorld()->GetTimeSeconds();

	// 前回発射から FireRate 秒以上経過していれば射撃実行
	if (CurrentTime - LastFireTime >= FireRate)
	{
		fire();

		// 最終発射時刻を更新（クールダウンリセット）
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

void ARaiden::OnQuitInput(const FInputActionValue& Value)
{
	UKismetSystemLibrary::QuitGame(this, PlayerController, EQuitPreference::Quit, true);
}
