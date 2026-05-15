// Fill out your copyright notice in the Description page of Project Settings.


#include "Raiden.h"

#include "InputMappingContext.h"

//UGameplayStatics
#include "Kismet/GameplayStatics.h"




ARaiden::ARaiden()
{
}

void ARaiden::BeginPlay()
{
	Super::BeginPlay();


	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
			{
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

