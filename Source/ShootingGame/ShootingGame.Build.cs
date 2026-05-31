// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ShootingGame : ModuleRules
{
	public ShootingGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",			// UE 核心类型（FString、TArray、UObject 基类等）
			"CoreUObject",		// UObject 反射系统（序列化、GC、运行时类型识别）
			"Engine",		// 引擎核心（Actor、Pawn、Component、GameMode 等游戏框架）
			"InputCore",		// 输入核心（按键枚举、轴映射配置）
			"EnhancedInput",	// 增强输入系统（InputAction、InputMappingContext）
			"UMG",			// UMG UI 框架（UserWidget、TextBlock 等控件）
			"Niagara"		// Niagara 粒子系统（VFX 粒子特效）
		});

        PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
