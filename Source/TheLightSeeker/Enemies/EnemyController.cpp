// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/EnemyController.h"
#include "CharacterBase.h"
#include "LightSeekerPlayerState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Enemies/EnemyBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameState.h"

AEnemyController::AEnemyController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bWantsPlayerState = true;
}

void AEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();

	if (UseBlackboard(BBAsset, BlackboardComponent))
	{
		UBehaviorTree* BTAsset = CastChecked<AEnemyBase>(InPawn)->GetBTAsset();
		if (!RunBehaviorTree(BTAsset))
		{
			UE_LOG(LogTemp, Error, TEXT("EnemyController couldn't run behavior tree!"));
		}
		else
		{
			float AttackRange = CastChecked<AEnemyBase>(InPawn)->GetAttackRange();
			BlackboardComponent->SetValueAsFloat(FName("AttackRange"), AttackRange);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed UseBlackboard()"));
	}
}
