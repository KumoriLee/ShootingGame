// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Components/CapsuleComponent.h"

#include "Projectile.h"


#include "BasePawn.generated.h"

UCLASS()
class SHOOTINGGAME_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();

	//コライダー
	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	//弾の生成点
	UPROPERTY(VisibleAnywhere)
	USceneComponent* ProjectileSpawnPoint;


	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> ProjectileClass;



	void fire();

	void HandleDestruction();

};
