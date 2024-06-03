// Copyright (c) 2023 Team Light Seekers All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ConsistentEnemySpawner.generated.h"

DECLARE_MULTICAST_DELEGATE(OnDeactivatedDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(OnEnemySpawnDelegate, TObjectPtr<class AEnemyBase>);

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	VE_Melee UMETA(DisplayName = "Melee"),
	VE_Range	 UMETA(DisplayName = "Range"),
	VE_Assassin	 UMETA(DisplayName = "Assassin"),
};

USTRUCT(BlueprintType)
struct FSpawnerInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FVector SpawnPosition;

	UPROPERTY(EditAnywhere)
	EEnemyType EnemyType;
};

UCLASS()
class THELIGHTSEEKER_API AConsistentEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AConsistentEnemySpawner();

	void Activate();
	void Deactivate();

	OnEnemySpawnDelegate OnEnemySpawnDelegateHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	bool		 LoadBP(const TCHAR* FilePath, TSubclassOf<class AEnemyBase> Target);

	void Spawn();

	UPROPERTY(EditAnywhere)
	TArray<FSpawnerInfo> SpawnerInfos;

	UPROPERTY(EditAnywhere)
	float SpawnDelay;

	bool IsActivated = false;

	FTimerHandle SpawnTimerHandle;

	OnDeactivatedDelegate OnDeactivateDelegateHandle;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AEnemyBase> MeleeEnemy;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AEnemyBase> RangeEnemy;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AEnemyBase>  AssassinEnemy;
};
