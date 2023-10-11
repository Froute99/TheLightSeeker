// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Enemies/BTNodes/BTDecorator_IsInAttackRange.h"
#include "Actors/Enemies/EnemyBase.h"
#include "Actors/Characters/CharacterBase.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
		return false;

	auto Target = Cast<ACharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("Player")));
	if (nullptr == Target)
		return false;

	bResult = (Target->GetDistanceTo(ControllingPawn) <= OwnerComp.GetBlackboardComponent()->GetValueAsFloat(FName("AttackRange")));
	return bResult;
}