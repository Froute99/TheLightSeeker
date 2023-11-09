// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Enemies/BTNodes/BTDecorator_IsInAttackRange.h"
#include "Actors/Enemies/EnemyBase.h"
#include "Actors/Characters/CharacterBase.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Is Player In Attack Range ?";
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return false;
	}

	auto Target = Cast<ACharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("Player")));
	if (nullptr == Target)
	{
		return false;
	}

	float AttackRange = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(FName("AttackRange"));
	float Distance = Target->GetDistanceTo(ControllingPawn);

	if (Distance > AttackRange)
	{
		bResult = false;
		return bResult;
	}

	float AngleInRadians = FMath::DegreesToRadians(AllowableFaceAngleDiff);
	float DotProductResult = FVector::DotProduct(ControllingPawn->GetActorForwardVector(),
		(Target->GetActorLocation() - ControllingPawn->GetActorLocation()).GetSafeNormal());

	bResult = (DotProductResult > 0.0f) && (cos(AngleInRadians) < DotProductResult);
	return bResult;
}