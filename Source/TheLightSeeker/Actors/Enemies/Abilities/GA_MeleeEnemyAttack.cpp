// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Enemies/Abilities/GA_MeleeEnemyAttack.h"
#include "AbilitySystemComponent.h"

UGA_MeleeEnemyAttack::UGA_MeleeEnemyAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	FGameplayTag AttackTag = FGameplayTag::RequestGameplayTag(FName("Ability.Attack"));
	AbilityTags.AddTag(AttackTag);
	ActivationOwnedTags.AddTag(AttackTag);
}

void UGA_MeleeEnemyAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	//UAnimMontage* MontageToPlay = MeleeAttackMontage;
	//
	//// Play fire montage and wait for event telling us to spawn the projectile
	//UGDAT_PlayMontageAndWaitForEvent* Task = UGDAT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, NAME_None, MontageToPlay, FGameplayTagContainer(), 1.0f, NAME_None, false, 1.0f);
	//Task->OnBlendOut.AddDynamic(this, &UGDGA_FireGun::OnCompleted);
	//Task->OnCompleted.AddDynamic(this, &UGDGA_FireGun::OnCompleted);
	//Task->OnInterrupted.AddDynamic(this, &UGDGA_FireGun::OnCancelled);
	//Task->OnCancelled.AddDynamic(this, &UGDGA_FireGun::OnCancelled);
	//Task->EventReceived.AddDynamic(this, &UGDGA_FireGun::EventReceived);
	//// ReadyForActivation() is how you activate the AbilityTask in C++. Blueprint has magic from K2Node_LatentGameplayTaskCall that will automatically call ReadyForActivation().
	//Task->ReadyForActivation();
}

void UGA_MeleeEnemyAttack::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
}

void UGA_MeleeEnemyAttack::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
}

void UGA_MeleeEnemyAttack::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
}
