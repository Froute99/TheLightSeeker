// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawnManager.h"	
#include "Engine/TriggerBase.h"
#include "Components/ShapeComponent.h"
#include "Actors/Characters/CharacterBase.h"

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
			DeactivateActors(Info.Actors);
			Info.Trigger->GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEnemySpawnManager::OnBeginOverlap);
		}
	}
}

void AEnemySpawnManager::ActivateActors(TArray<TObjectPtr<AActor>>& ActorHolder)
{
	for (AActor* Actor : ActorHolder)
	{
		UE_LOG(LogTemp, Log, TEXT("Child Activated"));
		Actor->SetActorEnableCollision(true);
		Actor->SetActorTickEnabled(true);
		Actor->SetActorHiddenInGame(false);
	}
}

void AEnemySpawnManager::DeactivateActors(TArray<TObjectPtr<AActor>>& ActorHolder)
{
	for (AActor* Actor : ActorHolder)
	{
		UE_LOG(LogTemp, Log, TEXT("Child Deactivated"));
		Actor->SetActorEnableCollision(false);
		Actor->SetActorTickEnabled(false);
		Actor->SetActorHiddenInGame(true);
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
				ActivateActors(Info.Actors);
				Info.Trigger->GetCollisionComponent()->OnComponentBeginOverlap.Clear();
			}
		}
	}
}