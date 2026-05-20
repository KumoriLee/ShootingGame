// Fill out your copyright notice in the Description page of Project Settings.


#include "Raiden.h"

#include "InputMappingContext.h"
//カメラのヘーダファイル
#include "Camera/CameraComponent.h"


//UGameplayStatics
#include "Kismet/GameplayStatics.h"




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


	//コントローラー初期化
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))//PlayerControllerを取得
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


		EIC->BindAction(FireAction, ETriggerEvent::Started, this, &ARaiden::FireInput);
	}
}

void ARaiden::MoveInput(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();

	FVector DeltaLocation = FVector(0.0f, 0.0f, 0.0f);
	DeltaLocation.X = Speed * InputValue * UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
	AddActorLocalOffset(DeltaLocation, true);
	DeltaLocation.Y = Speed * InputValue * UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
	AddActorLocalOffset(DeltaLocation, true);

	FRotator DeltaRotation = FRotator(0.0f, 0.0f, 0.0f);


}

void ARaiden::FireInput()
{
}

