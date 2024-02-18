// Copyright (c) 2023 Team Light Seekers All rights reserved.

#include "Enemies/BTNodes/BTTask_MoveToForSeconds.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MoveToForSeconds::UBTTask_MoveToForSeconds()
{
	NodeName = "Move to the actor for given seconds";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_MoveToForSeconds::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = UBTTask_MoveTo::ExecuteTask(OwnerComp, NodeMemory);

	FTimerHandle TimerHandle;

	// 람다 함수를 사용하여 타이머 만료 시 호출할 함수 정의
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateLambda([this, &OwnerComp]() {
		// 타이머 만료 후 실패로 처리, 여기서 &OwnerComp를 사용하여 OwnerComp에 접근할 수 있음
		OnTimerExpired(OwnerComp);
	});
	OwnerComp.GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, Timer, false);
	UE_LOG(LogTemp, Warning, TEXT("MoveToForSeconds ExecuteTask Called"));
	return Result;
}

void UBTTask_MoveToForSeconds::OnTimerExpired(UBehaviorTreeComponent& OwnerComp)
{
	UE_LOG(LogTemp, Warning, TEXT("MoveToForSeconds OnTimerExpired Called"));
	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
}
