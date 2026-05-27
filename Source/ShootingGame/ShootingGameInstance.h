// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LevelManagerSubsystem.h"
#include "ShootingGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAME_API UShootingGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	int32 LastLevelIndex = 3;

	UPROPERTY(EditAnywhere)
	int32 CurrentLevelIndex = 1;

	void LoadNextLevel();
	void RestartCurrentLevel();
	void RestartGame();

private:
	void Changelevel(int32 Index);

};
