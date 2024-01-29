// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Abilities/GA_EliteEnemyRage.h"
#include "AbilitySystemComponent.h"
#include "AT_PlayMontageAndWaitForEvent.h"
#include "Enemies/EnemyBase.h"

void UGA_EliteEnemyRage::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!AttackMontage)
	{
		UE_LOG(Enemy, Error, TEXT("MeleeEnemyAttack class does not have Montage to play"))
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	UAnimMontage* MontageToPlay = AttackMontage;

	// Play fire montage and wait for event telling us to spawn the projectile
	UAT_PlayMontageAndWaitForEvent* Task = UAT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, NAME_None, MontageToPlay, AttackMontageForWeapon, FGameplayTagContainer(), 1.0f, NAME_None, false, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &UGA_EliteEnemyRage::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &UGA_EliteEnemyRage::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &UGA_EliteEnemyRage::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &UGA_EliteEnemyRage::OnCancelled);
	Task->EventReceived.AddDynamic(this, &UGA_EliteEnemyRage::EventReceived);
	// ReadyForActivation() is how you activate the AbilityTask in C++. Blueprint has magic from K2Node_LatentGameplayTaskCall that will automatically call ReadyForActivation().
	Task->ReadyForActivation();

	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, true);
}

void UGA_EliteEnemyRage::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.EndAbility")))
	{
		OnCompleted(EventTag, EventData);
		return;
	}

	if (GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority
		&& EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.Enemy.Rage")))
	{
		FCollisionShape CollisionShape;
		CollisionShape.SetSphere(EffectRange);

		TArray<FHitResult> HitResults;
		GetWorld()->SweepMultiByChannel(HitResults, GetOwningActorFromActorInfo()->GetActorLocation(), GetOwningActorFromActorInfo()->GetActorLocation(), FQuat(), ECollisionChannel::ECC_Pawn, CollisionShape);

		FGameplayEffectSpecHandle RageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageGameplayEffect, GetAbilityLevel());

		for (FHitResult& Hit : HitResults)
		{
			AEnemyBase* Target = Cast<AEnemyBase>(Hit.GetActor());
			if (Target && !Target->IsHidden())
			{
				Target->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*RageEffectSpecHandle.Data);
			}
		}
	}
}
