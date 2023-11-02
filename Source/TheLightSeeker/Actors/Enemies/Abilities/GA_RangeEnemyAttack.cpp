// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Enemies/Abilities/GA_RangeEnemyAttack.h"
#include "AbilitySystemComponent.h"
#include "Actors/Enemies/Abilities/AT_PlayMontageAndWaitForEvent.h"
#include "Actors/Enemies/EnemyBase.h"
#include "Actors/Characters/LightSeekerPlayerState.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UGA_RangeEnemyAttack::UGA_RangeEnemyAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_RangeEnemyAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	UE_LOG(Enemy, Log, TEXT("RangeEnemyAttack called"));

	if (!RangeAttackMontage)
	{
		UE_LOG(Enemy, Error, TEXT("RangeEnemyAttack class does not have Montage to play"))
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	UAnimMontage* MontageToPlay = RangeAttackMontage;

	// Play fire montage and wait for event telling us to spawn the projectile
	UAT_PlayMontageAndWaitForEvent* Task = UAT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, NAME_None, MontageToPlay, FGameplayTagContainer(), 1.0f, NAME_None, false, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &UGA_RangeEnemyAttack::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &UGA_RangeEnemyAttack::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &UGA_RangeEnemyAttack::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &UGA_RangeEnemyAttack::OnCancelled);
	Task->EventReceived.AddDynamic(this, &UGA_RangeEnemyAttack::EventReceived);
	// ReadyForActivation() is how you activate the AbilityTask in C++. Blueprint has magic from K2Node_LatentGameplayTaskCall that will automatically call ReadyForActivation().
	Task->ReadyForActivation();
}

void UGA_RangeEnemyAttack::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	UE_LOG(Enemy, Log, TEXT("RangeEnemyAttack Cancelled"));
	SetAbilityDoneDelegate.Broadcast();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_RangeEnemyAttack::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	UE_LOG(Enemy, Log, TEXT("RangeEnemyAttack Completed"));
	SetAbilityDoneDelegate.Broadcast();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_RangeEnemyAttack::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	UE_LOG(Enemy, Log, TEXT("EventReceived called: %s"), EventTag.GetTagName());

	// Montage told us to end the ability before the montage finished playing.
	// Montage was set to continue playing animation even after ability ends so this is okay.
	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.EndAbility")))
	{
		OnCompleted(EventTag, EventData);
		//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	// Only spawn projectiles on the Server.
	// Predicting projectiles is an advanced topic not covered in this example.
	if (GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority
		&& EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.Enemy.RangeAttack")))
	{

		AEnemyBase* EnemyBase = Cast<AEnemyBase>(GetActorInfo().OwnerActor.Get());
		check(EnemyBase != nullptr);

		if (EnemyBase)
		{
			FHitResult Out;
			bool IsHitPlayer = GetWorld()->LineTraceSingleByChannel(Out,
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
				UE_LOG(Enemy, Log, TEXT("Attack event player hit %s"), *AActor::GetDebugName(Out.GetActor()));


				ACharacterBase* Player = Cast<ACharacterBase>(Out.GetActor());
				if (Player)
				{
					UE_LOG(Enemy, Log, TEXT("Attack event player hit2"));
					ALightSeekerPlayerState* PS = Cast<ALightSeekerPlayerState>(Player->GetPlayerState());

					if (PS)
					{
						UE_LOG(Enemy, Log, TEXT("Player HP before enemy attack: %f"), PS->GetHealth());

						FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageGameplayEffect, GetAbilityLevel());
						PS->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());


						UE_LOG(Enemy, Log, TEXT("Player HP after enemy attack: %f"), PS->GetHealth());

					}
				}
			}
		}
	}
}