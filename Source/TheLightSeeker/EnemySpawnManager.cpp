// Copyright (c) 2023 Team Light Seekers All rights reserved.

#include "EnemySpawnManager.h"
#include "Engine/TriggerBase.h"
#include "Components/ShapeComponent.h"
#include "Characters/CharacterBase.h"
#include "Enemies/EnemyBase.h"
#include "Math/UnrealMathUtility.h"
#include "Items/Item.h"
#include "ConsistentEnemySpawner.h"

// Sets default values
AEnemySpawnManager::AEnemySpawnManager()
{
	TotalWeight = 0;
	IsLastStage = false;
}

// Called when the game starts or when spawned
void AEnemySpawnManager::BeginPlay()
{
	Super::BeginPlay();

	for (FSpawnInfo& Info : TriggerActorPairs)
	{
		if (Info.Trigger)
		{
			DeactivateActors(Info.Enemies);
			Info.Trigger->GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEnemySpawnManager::OnBeginOverlap);
		}
	}

	if (!ItemDropTable.IsEmpty())
	{
		TotalWeight = 0;

		for (int i = 0; i < ItemDropTable.Num(); ++i)
		{
			TotalWeight += ItemDropTable[i].Weight;
			ItemDropTable[i].Weight = TotalWeight;
		}
		TotalWeight /= ItemDropRate;
	}
	
	NumOfActiveEnemies = 0;
	IsInCombat = false;

	for (AConsistentEnemySpawner* Spawner : BossStageEnemySpawners)
	{
		if (Spawner)
		{
			Spawner->OnEnemySpawnDelegateHandle.AddUObject(this, &AEnemySpawnManager::GiveItem);
		}
	}
}

void AEnemySpawnManager::GiveItem(TObjectPtr<AEnemyBase> EnemyToSpawn)
{
	if (ItemDropTable.Num() == 0)
	{
		return;
	}

	int32 RandomValue = FMath::RandRange(0, TotalWeight - 1);

	if (RandomValue < ItemDropTable.Last().Weight) // can drop item. otherwise - do not drop item
	{
		for (FItemDropInfo& DropInfo : ItemDropTable)
		{
			if (RandomValue < DropInfo.Weight)
			{
				EnemyToSpawn->Item = DropInfo.Item;
				return;
			}
		}
	}

	EnemyToSpawn->Item = nullptr;
}

void AEnemySpawnManager::ActivateActors(TArray<TObjectPtr<AEnemyBase>>& Enemies)
{
	for (TObjectPtr<AEnemyBase>& EnemyToSpawn : Enemies)
	{
		if (!EnemyToSpawn)
			continue;

		EnemyToSpawn->OnActivate();
		EnemyToSpawn->OnDiedDelegateHandle.AddDynamic(this, &AEnemySpawnManager::DecreaseNumOfActiveEnemies);
		
		++NumOfActiveEnemies;

		// some enemies might have fixed reward..
		if (!EnemyToSpawn->Item)
		{
			GiveItem(EnemyToSpawn);
		}
	}

	if (NumOfActiveEnemies > 0)
	{
		IsInCombat = true;
		if (OnCombatStatusChangedDelegateHandle.IsBound())
		{
			OnCombatStatusChangedDelegateHandle.Broadcast(IsInCombat);
		}
	}
}

void AEnemySpawnManager::DeactivateActors(TArray<TObjectPtr<AEnemyBase>>& Enemies)
{
	for (TObjectPtr<AEnemyBase>& EnemyToSpawn : Enemies)
	{
		if (!EnemyToSpawn)
			continue;
		EnemyToSpawn->OnDeactivate();
	}
}

void AEnemySpawnManager::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ACharacterBase>(OtherActor))
	{
		UE_LOG(LogTemp, Log, TEXT("Spawn Triggered"));
		AActor* Trigger = OverlappedComp->GetOwner();
		for (FSpawnInfo& Info : TriggerActorPairs)
		{
			if (Info.Trigger == Trigger)
			{
				if (Info.IsLastStage)
				{
					IsLastStage = true;
				}
				
				ActivateActors(Info.Enemies);
				Info.Trigger->GetCollisionComponent()->OnComponentBeginOverlap.Clear();
				Trigger->Destroy();

				if (Info.IsLastStage)
				{
					for (AConsistentEnemySpawner* Spawner : BossStageEnemySpawners)
					{
						Spawner->Activate();
					}
				}
			}
		}
	}
}

void AEnemySpawnManager::DecreaseNumOfActiveEnemies()
{
	--NumOfActiveEnemies;
	if (NumOfActiveEnemies == 0)
	{
		IsInCombat = false;
		
		if (IsLastStage)
		{
			IsBossDefeatedDelegateHandle.Broadcast();
			for (AConsistentEnemySpawner* Spawner : BossStageEnemySpawners)
			{
				Spawner->Deactivate();
			}

		}
		else
		{
			OnCombatStatusChangedDelegateHandle.Broadcast(IsInCombat);
		}
	}
}