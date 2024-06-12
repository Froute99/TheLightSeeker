// Copyright (c) 2023 Team Light Seekers All rights reserved.

#include "ConsistentEnemySpawner.h"
#include "EnemyBase.h"

// Sets default values
AConsistentEnemySpawner::AConsistentEnemySpawner()
{
	
}

void AConsistentEnemySpawner::Activate()
{
	if (IsActivated)
	{
		return;
	}

	IsActivated = true;

	FTimerDelegate TimerDelegate = FTimerDelegate::CreateLambda([this]() {
		if (this)
		{
			this->Spawn();
		}
	});
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, TimerDelegate, SpawnDelay, true, SpawnDelay);
}

void AConsistentEnemySpawner::Deactivate()
{
	IsActivated = false;
	GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
	OnDeactivateDelegateHandle.Broadcast();
}

// Called when the game starts or when spawned
void AConsistentEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	IsActivated = false;
}

void AConsistentEnemySpawner::Spawn()
{
	for (FSpawnerInfo& SpawnerInfo : SpawnerInfos)
	{
		FTransform Transform;
		Transform.SetLocation(SpawnerInfo.SpawnPosition);

		AEnemyBase* EnemyBase = nullptr;
		switch (SpawnerInfo.EnemyType)
		{
			case EEnemyType::VE_Melee:
				EnemyBase = Cast<AEnemyBase>(GetWorld()->SpawnActor(MeleeEnemy, &Transform));
				break;
			case EEnemyType::VE_Range:
				EnemyBase = Cast<AEnemyBase>(GetWorld()->SpawnActor(RangeEnemy, &Transform));
				break;
			case EEnemyType::VE_Assassin:
				EnemyBase = Cast<AEnemyBase>(GetWorld()->SpawnActor(AssassinEnemy, &Transform));
				break;
		}

		if (EnemyBase)
		{
			OnEnemySpawnDelegateHandle.Broadcast(EnemyBase);
		}
	}
}
