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
	/** * 动态设置背景的滚动速度。
	 * 供蓝图或 C++ 调用。例如：打 Boss 时传入 0 停止，起飞时传入 -0.5 加速。
	 */
	UFUNCTION(BlueprintCallable, Category = "Background")
	void SetScrollSpeed(float NewSpeed);

protected:
	// 背景网格体组件 (用来放 Plane)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Background")
	UStaticMeshComponent* BackgroundMesh;

	// 初始滚动速度 (负数表示背景向后退，视觉上飞机向前飞)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Background|Settings")
	float InitialScrollSpeed = -0.1f;

private:
	// 动态材质实例 (用于将参数下发给 GPU)
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMat;

	// 状态记录变量 (用于推导 UV 偏移公式)
	float CurrentSpeed;
	float BaseOffset;
	float LastSpeedChangeTime;

};
