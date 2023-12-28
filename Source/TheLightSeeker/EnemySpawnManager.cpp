// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawnManager.h"	
#include "Engine/TriggerBase.h"
#include "Components/ShapeComponent.h"
#include "Characters/CharacterBase.h"
#include "Enemies/EnemyBase.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AEnemySpawnManager::AEnemySpawnManager()
{

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
	}
}

void AEnemySpawnManager::GiveItem(TObjectPtr<AEnemyBase> EnemyToSpawn)
{
	int32 RandomValue = FMath::RandRange(0, (int32)(TotalWeight / ItemDropRate));

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
		if (!EnemyToSpawn) continue;

		EnemyToSpawn->SetActorEnableCollision(true);
		EnemyToSpawn->SetActorTickEnabled(true);
		EnemyToSpawn->SetActorHiddenInGame(false);

		if (AAIController* Controller = Cast<AAIController>(EnemyToSpawn->GetController()))
		{
			Controller->GetBrainComponent()->RestartLogic();
		}

		// Temp: give item based on the droptable
		GiveItem(EnemyToSpawn);
	}
}

void AEnemySpawnManager::DeactivateActors(TArray<TObjectPtr<AEnemyBase>>& Enemies)
{
	for (TObjectPtr<AEnemyBase>& EnemyToSpawn : Enemies)
	{
		if (!EnemyToSpawn) continue;

		EnemyToSpawn->SetActorEnableCollision(false);
		EnemyToSpawn->SetActorTickEnabled(false);
		EnemyToSpawn->SetActorHiddenInGame(true);

		if (AAIController* Controller = Cast<AAIController>(EnemyToSpawn->GetController()))
		{
			Controller->GetBrainComponent()->StopLogic(FString(TEXT("Waiting for activation..")));
		}
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
				ActivateActors(Info.Enemies);
				Info.Trigger->GetCollisionComponent()->OnComponentBeginOverlap.Clear();
			}
		}
	}
}