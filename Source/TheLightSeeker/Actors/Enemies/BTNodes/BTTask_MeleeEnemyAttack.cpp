// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MeleeEnemyAttack.h"

EBTNodeResult::Type UBTTask_MeleeEnemyAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	UE_LOG(LogTemp, Log, TEXT("MeleeEnemyAttack Execute called"));

	return Result;
}
