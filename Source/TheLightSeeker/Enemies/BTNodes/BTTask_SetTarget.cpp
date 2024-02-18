// Copyright (c) 2023 Team Light Seekers All rights reserved.

#include "Enemies/BTNodes/BTTask_SetTarget.h"
#include "Enemies/EnemyBase.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/GameState.h"
#include "Characters/LightSeekerPlayerState.h"

EBTNodeResult::Type UBTTask_SetTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyBase* EnemyBase = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!EnemyBase)
	{
		UE_LOG(Enemy, Log, TEXT("UBTTask_SetTarget Failed with no EnemyBase"));
		return EBTNodeResult::Failed;
	}

	if (EnemyBase->GetTargetPlayer())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName("Target"), EnemyBase->GetTargetPlayer());
	}
	else
	{
		// TODO: Fix to check ALightSeekerPlayerState

		// Set random target
		TArray<TObjectPtr<APlayerState>> Players;
		int								 TotalPlayerNum = GetWorld()->GetGameState()->PlayerArray.Num();

		if (TotalPlayerNum <= 0)
		{
			return EBTNodeResult::Failed;
		}

		for (int i = 0; i < TotalPlayerNum; ++i)
		{
			if (GetWorld()->GetGameState()->PlayerArray[i]->GetPlayerController())
			{
				Players.Add(GetWorld()->GetGameState()->PlayerArray[i]);
			}
		}

		int RandomIndex = FMath::RandRange(0, Players.Num() - 1);

		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName("Target"),
			Cast<ACharacterBase>(Players[RandomIndex]->GetPawn()));
	}

	// Force Fail to running next task of root selector.
	return EBTNodeResult::Failed;
}
