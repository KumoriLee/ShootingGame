// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"

#include "EnhancedInputSubsystems.h"

#include "InputAction.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"


#include "Raiden.generated.h"
//入力対応
class UInputMappingContext;


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

	//デフォルト値
	UPROPERTY(EditAnywhere)
	float Speed = 300.0f;

	//メソッド
	void MoveInput(const FInputActionValue& Value);
	void FireInput();
	
};
