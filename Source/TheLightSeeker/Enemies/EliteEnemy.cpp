// Copyright (c) 2023 Team Light Seekers All rights reserved.

#include "EliteEnemy.h"
#include "GameAbilitySystem/CharacterAbilitySystemComponent.h"
#include "GameAbilitySystem/CharacterAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/GameState.h"
#include "Characters/LightSeekerPlayerState.h"
#include "Item.h"

AEliteEnemy::AEliteEnemy()
{
	//HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HP Display Bar"));
	//HPBar->SetupAttachment(RootComponent);
}

void AEliteEnemy::DropItem()
{
	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (Item)
	{
		FActorSpawnParameters Parameter{};
		Parameter.Instigator = this;
		FVector LaunchLocation = GetActorLocation();
		LaunchLocation.Z = LaunchLocation.Z - GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 100.0;

		// Set random target
		TArray<TObjectPtr<APlayerState>> Players;
		int								 TotalPlayerNum = GetWorld()->GetGameState()->PlayerArray.Num();

		for (int i = 0; i < TotalPlayerNum; ++i)
		{
			if (GetWorld()->GetGameState()->PlayerArray[i]->GetPlayerController())
			{
				Players.Add(GetWorld()->GetGameState()->PlayerArray[i]);
			}
		}

		for (TObjectPtr<APlayerState>& Player : Players)
		{
			UE_LOG(Enemy, Log, TEXT("SkillItem spawned"));
			AItem* SpawnedItem = GetWorld()->SpawnActor<AItem>(Item, LaunchLocation, FRotator(), Parameter);
			SpawnedItem->SetOwner(Player->GetOwningController());
		}
	}
}