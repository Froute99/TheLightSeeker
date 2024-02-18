// Copyright (c) 2023 Team Light Seekers All rights reserved.

#include "BTNodes/BTDecorator_IsCooldownDone.h"
#include "Abilities/GameplayAbility.h"
#include "Enemies/EnemyBase.h"
#include "AIController.h"

UBTDecorator_IsCooldownDone::UBTDecorator_IsCooldownDone(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Is Cooldown for ability done and can activate it?";
}

bool UBTDecorator_IsCooldownDone::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AEnemyBase* EnemyBase = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!EnemyBase)
	{
		return false;
	}

	FGameplayTagContainer TagContainer;
	EnemyBase->GetAbilitySystemComponent()->GetOwnedGameplayTags(TagContainer);

	for (int i = 0; i < TagContainer.Num(); ++i)
	{
		FGameplayTag Tag = TagContainer.GetByIndex(i);
		if (Tag == CooldownTag) // has cooldown tag = still in cooldown -> run MoveTo Node
		{
			return true;
		}
	}

	return false;
}
