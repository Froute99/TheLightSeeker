// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnManager.generated.h"

USTRUCT(BlueprintType)
struct FSpawnInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<class ATriggerBase> Trigger;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<class AEnemyBase>> Enemies;
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
};
