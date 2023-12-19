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
	TArray<TObjectPtr<AActor>> Actors;
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

public:	
	void ActivateActors(TArray<TObjectPtr<AActor>>& ActorHolder);
	void DeactivateActors(TArray<TObjectPtr<AActor>>& ActorHolder);

	UPROPERTY(EditAnywhere)
	TArray<FSpawnInfo> TriggerActorPairs;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
