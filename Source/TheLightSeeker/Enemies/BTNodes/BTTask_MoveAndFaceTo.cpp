// Copyright (c) 2023 Team Light Seekers All rights reserved.

#include "Enemies/BTNodes/BTTask_MoveAndFaceTo.h"
#include "Characters/CharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/EnemyBase.h"
#include "AIController.h"
#include "Tasks/AITask_MoveTo.h"

UBTTask_MoveAndFaceTo::UBTTask_MoveAndFaceTo()
{
	NodeName = "Move to the target until reach & face";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_MoveAndFaceTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBTTask_MoveTo::AcceptableRadius = 1.0f; // to make AI to face target.

	EBTNodeResult::Type Result = UBTTask_MoveTo::ExecuteTask(OwnerComp, NodeMemory);

	if (Result == EBTNodeResult::Type::Succeeded && !IsTaskDone(OwnerComp))
	{
		Result = EBTNodeResult::Type::InProgress;
	}

	return Result;
}

void UBTTask_MoveAndFaceTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (IsTaskDone(OwnerComp))
	{
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
		UE_LOG(Enemy, Log, TEXT("BTTAsk: %f, %f"), Distance, AttackRange);
		return false;
	}

	FVector ToTarget = (Target->GetActorLocation() - ControllingPawn->GetActorLocation());
	ToTarget.Z = 0.0f;
	ToTarget.Normalize();

	float AngleInRadians = FMath::DegreesToRadians(AcceptableFaceAngleDiff);
	float DotProductResult = FVector::DotProduct(ControllingPawn->GetActorForwardVector(), ToTarget);

	UE_LOG(Enemy, Log, TEXT("BTTAsk2: %f, %f"), AngleInRadians, DotProductResult);
	if ((DotProductResult >= 0.0f) && (cos(AngleInRadians) < DotProductResult))
	{
		return true;
	}
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return false;
	}
}
