// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingGameMode.h"
#include "Kismet/GameplayStatics.h"


void AShootingGameMode::BeginPlay()
{
	Super::BeginPlay();


	//gamemode§Ú π”√§∑§∆•«•’•©•Î•»•◊•Ï•§•‰©`§Ú‘O∂®§π§Î
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		raiden = Cast<ARaiden>(PlayerPawn);
		if (!raiden)
		{
			UE_LOG(LogTemp, Display, TEXT("GameMode: Failed to find the player actor!"));
		}
	}

}
