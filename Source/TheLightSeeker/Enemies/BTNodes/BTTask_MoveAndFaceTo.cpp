// Copyright (c) 2023 Team Light Seekers All rights reserved.

#include "Enemies/BTNodes/BTTask_MoveAndFaceTo.h"
#include "Characters/CharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/EnemyBase.h"
#include "AIController.h"

UBTTask_MoveAndFaceTo::UBTTask_MoveAndFaceTo()
{
	NodeName = "Move to the target until reach & face";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_MoveAndFaceTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBTTask_MoveTo::AcceptableRadius = 1.0f; // to make AI to face target.

	UE_LOG(Enemy, Warning, TEXT("MoveAndFaceTo1"));
	EBTNodeResult::Type Result = UBTTask_MoveTo::ExecuteTask(OwnerComp, NodeMemory);
	UE_LOG(Enemy, Warning, TEXT("MoveAndFaceTo2"));

	if (Result == EBTNodeResult::Type::Succeeded && !IsTaskDone(OwnerComp))
	{
		UE_LOG(Enemy, Warning, TEXT("MoveAndFaceTo3"));
		Result = EBTNodeResult::Type::InProgress;
	}

	return Result;
}

void UBTTask_MoveAndFaceTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (IsTaskDone(OwnerComp))
	{
		UE_LOG(Enemy, Warning, TEXT("MoveAndFaceTo4"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

bool UBTTask_MoveAndFaceTo::IsTaskDone(UBehaviorTreeComponent& OwnerComp) const
{
	auto Target = Cast<ACharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("Target")));
	auto ControllingPawn = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());

	if (Target == nullptr)
	{
		return false;
	}

	float AttackRange = AcceptableRadiusForNode; //OwnerComp.GetBlackboardComponent()->GetValueAsFloat(FName("AttackRange"));
	float Distance = Target->GetDistanceTo(ControllingPawn);

	if (Distance > AttackRange)
	{
		return false;
	}

	FVector ToTarget = (Target->GetActorLocation() - ControllingPawn->GetActorLocation());
	ToTarget.Z = 0.0f;
	ToTarget.Normalize();

	float AngleInRadians = FMath::DegreesToRadians(AcceptableFaceAngleDiff);
	float DotProductResult = FVector::DotProduct(ControllingPawn->GetActorForwardVector(), ToTarget);

	return (DotProductResult >= 0.0f) && (cos(AngleInRadians) < DotProductResult);
}