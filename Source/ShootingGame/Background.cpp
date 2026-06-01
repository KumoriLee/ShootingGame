// Fill out your copyright notice in the Description page of Project Settings.


#include "Background.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABackground::ABackground()
{
	PrimaryActorTick.bCanEverTick = false;

	// 创建网格体并设为根节点
	BackgroundMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackgroundMesh"));
	RootComponent = BackgroundMesh;

	// 【物理与渲染优化】背景板不需要碰撞，不产生阴影，不接收贴花
	BackgroundMesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	BackgroundMesh->SetCastShadow(false);
	BackgroundMesh->bReceivesDecals = false;

	// 变量初始化
	CurrentSpeed = InitialScrollSpeed;
	BaseOffset = 0.0f;
	LastSpeedChangeTime = 0.0f;
}

// Called when the game starts or when spawned
void ABackground::BeginPlay()
{
	Super::BeginPlay();
	
	// 确保模型上挂载了材质
	if (BackgroundMesh->GetMaterial(0) != nullptr)
	{
		// 创建动态材质实例
		DynamicMat = BackgroundMesh->CreateAndSetMaterialInstanceDynamic(0);

		// 游戏开始时，将当前世界时间同步给材质
		LastSpeedChangeTime = GetWorld()->GetTimeSeconds();
		SetScrollSpeed(InitialScrollSpeed);
	}
}

void ABackground::SetScrollSpeed(float NewSpeed)
{
	// 如果材质还没准备好，或者请求的速度和当前速度一样，则不消耗性能去计算
	if (!DynamicMat || FMath::IsNearlyEqual(CurrentSpeed, NewSpeed))
	{
		return;
	}

	// 获取当前世界的绝对时间 (支持游戏暂停的时间系统)
	float CurrentGameTime = GetWorld()->GetTimeSeconds();

	// 1. 结算上一段速度运行至今，积累的总 UV 偏移量
	BaseOffset += (CurrentGameTime - LastSpeedChangeTime) * CurrentSpeed;

	// 【防抖动处理】只保留小数部分。
	// 因为贴图是循环的(Wrap)，偏移 100.2 和偏移 0.2 画面是一模一样的。
	// 但如果数值过大（如玩了几个小时），浮点数精度丢失会导致画面剧烈抽搐。
	BaseOffset = FMath::Fmod(BaseOffset, 1.0f);
	if (BaseOffset < 0.0f)
	{
		BaseOffset += 1.0f; // 保证数值永远在 0.0 ~ 1.0 之间
	}

	// 2. 更新状态：保存新的速度，以及发生速度改变的"当前时间"
	CurrentSpeed = NewSpeed;
	LastSpeedChangeTime = CurrentGameTime;

	// 3. 将计算好的新起点、新时间、新速度下发给材质 (GPU 将接管后续所有的滚动计算)
	DynamicMat->SetScalarParameterValue(FName("BaseOffset"), BaseOffset);
	DynamicMat->SetScalarParameterValue(FName("StartTime"), LastSpeedChangeTime);
	DynamicMat->SetScalarParameterValue(FName("Speed"), CurrentSpeed);
}



