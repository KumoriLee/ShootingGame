// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LevelManagerSubsystem.h"
#include "ShootingGameInstance.generated.h"

/**
 * シューティングゲームの GameInstance
 * LevelManagerSubsystem を介したレベル遷移のラッパーを提供
 */
UCLASS()
class SHOOTINGGAME_API UShootingGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	/** 最終レベルのインデックス（1ベース） */
	UPROPERTY(EditAnywhere)
	int32 LastLevelIndex = 3;

	/** 現在のレベルインデックス（1ベース） */
	UPROPERTY(EditAnywhere)
	int32 CurrentLevelIndex = 1;

	/** 次のレベルをロードする */
	void LoadNextLevel();
	/** 現在のレベルをリロードする */
	void RestartCurrentLevel();
	/** 最初のレベルからやり直す */
	void RestartGame();

private:
	/** 指定インデックスのレベルに遷移する（1ベース → LevelManager 用に0ベース変換） */
	void Changelevel(int32 Index);

};
