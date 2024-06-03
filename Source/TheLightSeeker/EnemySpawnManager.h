// Copyright (c) 2023 Team Light Seekers All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCombatStatusChangedDelegate, bool, IsInCombat);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossDefeatedDelegate);

USTRUCT(BlueprintType)
struct FSpawnInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<class ATriggerBase> Trigger;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<class AEnemyBase>> Enemies;

	UPROPERTY(EditAnywhere)
	bool IsLastStage;
};

USTRUCT(BlueprintType)
struct FItemDropInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AItem> Item;

	UPROPERTY(EditAnywhere)
	int Weight;
};

UCLASS()
class THELIGHTSEEKER_API AEnemySpawnManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemySpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void GiveItem(TObjectPtr<AEnemyBase> EnemyToSpawn);

public:
	void ActivateActors(TArray<TObjectPtr<AEnemyBase>>& Enemies);
	void DeactivateActors(TArray<TObjectPtr<AEnemyBase>>& Enemies);

	UPROPERTY(EditAnywhere)
	TArray<FSpawnInfo> TriggerActorPairs;

	UPROPERTY(EditAnywhere, Category = "Item")
	float ItemDropRate;

	UPROPERTY(EditAnywhere, Category = "Item")
	TArray<FItemDropInfo> ItemDropTable;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	int32 TotalWeight;

protected:
	UFUNCTION()
	void DecreaseNumOfActiveEnemies();

	int32 NumOfActiveEnemies;
	
	bool IsInCombat;

	bool IsLastStage;

	UPROPERTY(EditAnywhere)
	TArray<class AConsistentEnemySpawner*> BossStageEnemySpawners;

public:
	UPROPERTY(BlueprintAssignable)
	FOnCombatStatusChangedDelegate OnCombatStatusChangedDelegateHandle;

	UPROPERTY(BlueprintAssignable)
	FOnBossDefeatedDelegate IsBossDefeatedDelegateHandle;
};
