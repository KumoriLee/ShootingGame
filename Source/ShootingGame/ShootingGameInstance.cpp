// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingGameInstance.h"

void UShootingGameInstance::Changelevel(int32 Index)
{
	// LevelManagerSubsystem は0ベースインデックスを使用するため、1ベースから変換
	if (ULevelManagerSubsystem* LM = GetSubsystem<ULevelManagerSubsystem>())
	{
		LM->OpenLevelByIndex(Index - 1);
	}
}

void UShootingGameInstance::LoadNextLevel()
{
	if (ULevelManagerSubsystem* LM = GetSubsystem<ULevelManagerSubsystem>())
	{
		LM->OpenNextLevel();
	}
}

void UShootingGameInstance::RestartCurrentLevel()
{
	if (ULevelManagerSubsystem* LM = GetSubsystem<ULevelManagerSubsystem>())
	{
		LM->RestartCurrentLevel();
	}
}

void UShootingGameInstance::RestartGame()
{
	if (ULevelManagerSubsystem* LM = GetSubsystem<ULevelManagerSubsystem>())
	{
		LM->RestartGame();
	}
}
