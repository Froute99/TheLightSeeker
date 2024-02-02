// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BTNodes/BTTask_TriggerAbilityByClass.h"
#include "CharacterAbilitySystemComponent.h"
#include "AIController.h"
#include "Enemies/EnemyBase.h"
#include "Enemies/Abilities/EnemyGameplayAbility.h"
#include "Enemies/Abilities/GA_RangeEnemyAttack.h"
#include "Enemies/Abilities/GA_BossEnemyLeap.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TriggerAbilityByClass::UBTTask_TriggerAbilityByClass()
{
	NodeName = "Trigger Ability By Class";
	bNotifyTick = true;
	RunningAbility = nullptr;
}

EBTNodeResult::Type UBTTask_TriggerAbilityByClass::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	if (!AbilityToActivate)
	{
		UE_LOG(Enemy, Log, TEXT("Has no ability to trigger"));
		return AbortTask(OwnerComp, NodeMemory);
	}

	AEnemyBase* OwnerEnemy = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (OwnerEnemy == nullptr)
	{
		UE_LOG(Enemy, Log, TEXT("Actor is not an enemy"));
		return AbortTask(OwnerComp, NodeMemory);
	}

	UAbilitySystemComponent* ASC = OwnerEnemy->GetAbilitySystemComponent();
	if (!ASC->TryActivateAbilityByClass(AbilityToActivate))
	{
		UE_LOG(Enemy, Log, TEXT("Could not activate ability"));
		return EBTNodeResult::Type::Failed; //AbortTask(OwnerComp, NodeMemory); fix for cooldown
	}
	else
	{
		IsTaskDone = false;
		RunningAbility = Cast<UEnemyGameplayAbility>(ASC->GetAnimatingAbility());
		Cast<UEnemyGameplayAbility>(RunningAbility)->SetAbilityDoneDelegateHandle.AddUFunction(this, FName("SetTaskDone"));

		if (RunningAbility->IsRequireTargetReference)
		{
			RunningAbility->SetTargetReference(TWeakObjectPtr<AActor>(Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("Target"))));
		}
	}

	return EBTNodeResult::Type::InProgress;
}

void UBTTask_TriggerAbilityByClass::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (IsTaskDone)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Type::Succeeded);
	}
}

void UBTTask_TriggerAbilityByClass::SetTaskDone()
{
	IsTaskDone = true;
	RunningAbility->SetAbilityDoneDelegateHandle.Clear();
	RunningAbility->SetTargetReference(nullptr);
}
 