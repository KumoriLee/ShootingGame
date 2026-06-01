// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Background.generated.h"

UCLASS()
class SHOOTINGGAME_API ABackground : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABackground();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	/**
	 * 背景のスクロール速度を動的に設定する
	 * ブループリントまたは C++ から呼び出し可能。例：ボス戦で 0 を渡して停止、離陸時に -0.5 を渡して加速。
	 */
	UFUNCTION(BlueprintCallable, Category = "Background")
	void SetScrollSpeed(float NewSpeed);

protected:
	/** 背景メッシュコンポーネント（Plane を配置） */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Background")
	UStaticMeshComponent* BackgroundMesh;

	/** 初期スクロール速度（負の値で背景が後退し、視覚的に機体が前進する） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Background|Settings")
	float InitialScrollSpeed = -0.1f;

private:
	/** 動的マテリアルインスタンス（パラメータを GPU に送るために使用） */
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMat;

	/** UV オフセット計算のための内部状態変数 */
	float CurrentSpeed;
	float BaseOffset;
	float LastSpeedChangeTime;

};
