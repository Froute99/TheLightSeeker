// Copyright (c) 2023 Team Light Seekers All rights reserved.

#include "Enemies/Abilities/GA_BossEnemyAttack.h"
#include "AbilitySystemComponent.h"
#include "AT_PlayMontageAndWaitForEvent.h"
#include "EnemyBase.h"
#include "LightSeekerPlayerState.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"

void UGA_BossEnemyAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!AttackMontage)
	{
		UE_LOG(Enemy, Error, TEXT("BossEnemyAttack class does not have Montage to play"))
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
	Task->OnBlendOut.AddDynamic(this, &UGA_BossEnemyAttack::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &UGA_BossEnemyAttack::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &UGA_BossEnemyAttack::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &UGA_BossEnemyAttack::OnCancelled);
	Task->EventReceived.AddDynamic(this, &UGA_BossEnemyAttack::EventReceived);
	// ReadyForActivation() is how you activate the AbilityTask in C++. Blueprint has magic from K2Node_LatentGameplayTaskCall that will automatically call ReadyForActivation().
	Task->ReadyForActivation();
}

void UGA_BossEnemyAttack::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	// Montage told us to end the ability before the montage finished playing.
	// Montage was set to continue playing animation even after ability ends so this is okay.
	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.EndAbility")))
	{
		OnCompleted(EventTag, EventData);
		// EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
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
			FHitResult Out;
			bool	   IsHitPlayer = GetWorld()->LineTraceSingleByChannel(Out,
					  EnemyBase->GetActorLocation() + EnemyBase->GetActorForwardVector() * EnemyBase->GetCapsuleComponent()->GetScaledCapsuleRadius() / 2.0f,
					  EnemyBase->GetActorLocation() + EnemyBase->GetActorForwardVector() * EnemyBase->GetAttackRange(),
					  ECollisionChannel::ECC_GameTraceChannel1);

			// DEBUG
			UKismetSystemLibrary::DrawDebugLine(GetWorld(), EnemyBase->GetActorLocation() + EnemyBase->GetActorForwardVector() * EnemyBase->GetCapsuleComponent()->GetScaledCapsuleRadius() / 2.0f,
				EnemyBase->GetActorLocation() + EnemyBase->GetActorForwardVector() * EnemyBase->GetAttackRange(), FLinearColor::Blue, 5.0f);
			UKismetSystemLibrary::DrawDebugCapsule(GetWorld(), EnemyBase->GetActorLocation() + EnemyBase->GetActorForwardVector() * EnemyBase->GetCapsuleComponent()->GetScaledCapsuleRadius() / 2.0f, 50.0f, 50.0f, FRotator(), FLinearColor::Blue, 5.0f);
			UKismetSystemLibrary::DrawDebugCapsule(GetWorld(), EnemyBase->GetActorLocation() + EnemyBase->GetActorForwardVector() * EnemyBase->GetAttackRange(), 50.0f, 50.0f, FRotator(), FLinearColor::Blue, 5.0f);

			if (IsHitPlayer)
			{
				// UE_LOG(Enemy, Log, TEXT("Attack event player hit %s"), *AActor::GetDebugName(Out.GetActor()));

				ACharacterBase* Player = Cast<ACharacterBase>(Out.GetActor());
				if (Player)
				{
					// UE_LOG(Enemy, Log, TEXT("Attack event player hit2"));
					ALightSeekerPlayerState* PS = Cast<ALightSeekerPlayerState>(Player->GetPlayerState());

					if (PS)
					{
						// UE_LOG(Enemy, Log, TEXT("Player HP before enemy attack: %f"), PS->GetHealth());

						FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageGameplayEffect, GetAbilityLevel());
						// PS->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
						EnemyBase->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*DamageEffectSpecHandle.Data.Get(), PS->GetAbilitySystemComponent());
					}
				}
			}
		}
	}
}
