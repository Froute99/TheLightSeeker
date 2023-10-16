// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Enemies/EnemyController.h"
#include "Actors/Characters/CharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Actors/Enemies/EnemyBase.h"
#include "Kismet/GameplayStatics.h"

void AEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();

	UE_LOG(LogTemp, Error, TEXT("Running OnPossess.."));
	if (UseBlackboard(BBAsset, BlackboardComponent))
	{
		UBehaviorTree* BTAsset = CastChecked<AEnemyBase>(InPawn)->GetBTAsset();
		if (!RunBehaviorTree(BTAsset))
		{
			UE_LOG(LogTemp, Error, TEXT("EnemyController couldn't run behavior tree!"));
		}
		else
		{
			if (!BlackboardComponent->GetValueAsObject(FName("Player")))
			{
				TArray<AActor*> actors;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacterBase::StaticClass(), actors);
				
				check(actors.Num() >= 1);

				BlackboardComponent->SetValueAsObject(FName("Player"), actors[0]);
				UE_LOG(LogTemp, Log, TEXT("Registered Player into blackboard successfully"));
			}

			float AttackRange = CastChecked<AEnemyBase>(InPawn)->GetAttackRange();
			BlackboardComponent->SetValueAsFloat(FName("AttackRange"), AttackRange);
			UE_LOG(LogTemp, Log, TEXT("Registered Enemy AttackRange successfully"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed UseBlackboard()"));
	}
}
