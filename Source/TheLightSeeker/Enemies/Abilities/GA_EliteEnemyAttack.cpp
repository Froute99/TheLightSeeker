// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Abilities/GA_EliteEnemyAttack.h"
#include "AbilitySystemComponent.h"
#include "AT_PlayMontageAndWaitForEvent.h"
#include "EnemyBase.h"
#include "LightSeekerPlayerState.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AT_RotateToTarget.h"
#include "Components/BoxComponent.h"

void UGA_EliteEnemyAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!AttackMontage)
	{
		UE_LOG(Enemy, Error, TEXT("EliteEnemyAttack class does not have Montage to play"))
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
	Task->OnBlendOut.AddDynamic(this, &UGA_EliteEnemyAttack::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &UGA_EliteEnemyAttack::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &UGA_EliteEnemyAttack::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &UGA_EliteEnemyAttack::OnCancelled);
	Task->EventReceived.AddDynamic(this, &UGA_EliteEnemyAttack::EventReceived);
	// ReadyForActivation() is how you activate the AbilityTask in C++. Blueprint has magic from K2Node_LatentGameplayTaskCall that will automatically call ReadyForActivation().
	Task->ReadyForActivation();
}

void UGA_EliteEnemyAttack::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	// Montage told us to end the ability before the montage finished playing.
	// Montage was set to continue playing animation even after ability ends so this is okay.
	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.EndAbility")))
	{
		OnCompleted(EventTag, EventData);
		//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	// stop rotation if needed
	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.StopRotation")))
	{
		if (RotateTask && RotateTask->IsActive())
		{
			RotateTask->EndTask();
		}
		return;
	}

	// Only spawn projectiles on the Server.
	// Predicting projectiles is an advanced topic not covered in this example.
	if (GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority
		&& EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.Enemy.MeleeAttack")))
	{
		AEnemyBase* EnemyBase = Cast<AEnemyBase>(GetActorInfo().OwnerActor.Get());
		check(EnemyBase != nullptr);

		if (EnemyBase)
		{
			TSet<AActor*> OverlappingActors;
			EnemyBase->MeleeAttackCollisionVolume->GetOverlappingActors(OverlappingActors, ACharacterBase::StaticClass());

			for (AActor* Actor : OverlappingActors)
			{
				if (ACharacterBase* Player = Cast<ACharacterBase>(Actor))
				{
					if (Player->IsInvincible())
					{
						continue;
					}

					ALightSeekerPlayerState* PS = Cast<ALightSeekerPlayerState>(Player->GetPlayerState());
					if (PS)
					{
						FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageGameplayEffect, GetAbilityLevel());
						EnemyBase->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*DamageEffectSpecHandle.Data.Get(), PS->GetAbilitySystemComponent());
					}
				}
			}
		}
	}
}
