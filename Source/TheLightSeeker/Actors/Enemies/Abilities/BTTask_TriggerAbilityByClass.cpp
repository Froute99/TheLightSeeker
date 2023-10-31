// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Enemies/Abilities/BTTask_TriggerAbilityByClass.h"
#include "CharacterAbilitySystemComponent.h"
#include "AIController.h"
#include "Actors/Enemies/EnemyBase.h"
#include "Enemies/Abilities/GA_MeleeEnemyAttack.h"

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
		return AbortTask(OwnerComp, NodeMemory);
	}
	else
	{
		IsTaskDone = false;
		RunningAbility = ASC->GetAnimatingAbility();
		if (UGA_MeleeEnemyAttack* Ability = Cast<UGA_MeleeEnemyAttack>(RunningAbility))
		{
			Ability->SetAbilityDoneDelegate.AddUFunction(this, FName("SetTaskDone"));
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
	UE_LOG(Enemy, Log, TEXT("UBTTask_TriggerAbilityByClass SetTaskDone called"));
	IsTaskDone = true;
	if (UGA_MeleeEnemyAttack* Ability = Cast<UGA_MeleeEnemyAttack>(RunningAbility))
	{
		Ability->SetAbilityDoneDelegate.Clear();
	}
}
 