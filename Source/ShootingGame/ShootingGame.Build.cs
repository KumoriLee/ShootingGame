// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ShootingGame : ModuleRules
{
	public ShootingGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",			// UE コア型（FString、TArray、UObject 基底クラスなど）
			"CoreUObject",		// UObject リフレクションシステム（シリアライズ、GC、ランタイム型識別）
			"Engine",		// エンジンコア（Actor、Pawn、Component、GameMode などのゲームフレームワーク）
			"InputCore",		// 入力コア（キー列挙型、軸マッピング設定）
			"EnhancedInput",	// EnhancedInput システム（InputAction、InputMappingContext）
			"UMG",			// UMG UI フレームワーク（UserWidget、TextBlock などのウィジェット）
			"Niagara"		// Niagara パーティクルシステム（VFX パーティクルエフェクト）
		});

        PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
