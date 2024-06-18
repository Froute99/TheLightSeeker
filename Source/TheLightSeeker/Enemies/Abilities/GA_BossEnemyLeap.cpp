// Copyright (c) 2023 Team Light Seekers All rights reserved.

#include "Enemies/Abilities/GA_BossEnemyLeap.h"
#include "AbilitySystemComponent.h"
#include "AT_PlayMontageAndWaitForEvent.h"
#include "EnemyBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AT_RotateToTarget.h"
#include "LightSeekerPlayerState.h"

void UGA_BossEnemyLeap::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!AttackMontage)
	{
		UE_LOG(Enemy, Error, TEXT("UGA_BossEnemyLeap class does not have Montage to play"))
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	UAnimMontage* MontageToPlay = AttackMontage;

	// Play fire montage and wait for event telling us to spawn the projectile
	UAT_PlayMontageAndWaitForEvent* Task = UAT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, NAME_None, MontageToPlay, AttackMontageForWeapon, FGameplayTagContainer(), 1.0f, NAME_None, false, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &UGA_BossEnemyLeap::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &UGA_BossEnemyLeap::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &UGA_BossEnemyLeap::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &UGA_BossEnemyLeap::OnCancelled);
	Task->EventReceived.AddDynamic(this, &UGA_BossEnemyLeap::EventReceived);
	// ReadyForActivation() is how you activate the AbilityTask in C++. Blueprint has magic from K2Node_LatentGameplayTaskCall that will automatically call ReadyForActivation().
	Task->ReadyForActivation();
}

void UGA_BossEnemyLeap::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.EndAbility")))
	{
		OnCompleted(EventTag, EventData);
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

	if (GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority
		&& EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.Enemy.Boss.Leap.Start")))
	{
		FVector CurrentLocation = GetOwningActorFromActorInfo()->GetActorLocation();
		CurrentLocation.Z = 0.0f;

		if (AbilityTarget.IsValid())
		{
			TargetLocation = AbilityTarget->GetActorLocation();
		}

		FVector						 LeapVelocity = (TargetLocation - CurrentLocation) / LeapDuration;
		UCharacterMovementComponent* EnemyCharacterMovement = Cast<ACharacter>(GetOwningActorFromActorInfo())->GetCharacterMovement();
		Cast<ACharacter>(GetOwningActorFromActorInfo())->FaceRotation(LeapVelocity.Rotation());
		EnemyCharacterMovement->Velocity = LeapVelocity;
		EnemyCharacterMovement->DoJump(false);
	}

	if (GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority
		&& EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.Enemy.Boss.Leap.End")))
	{
		UCharacterMovementComponent* EnemyCharacterMovement = Cast<ACharacter>(GetOwningActorFromActorInfo())->GetCharacterMovement();
		EnemyCharacterMovement->Velocity = FVector::ZeroVector;
	}

	if (GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority
		&& EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.Enemy.Boss.Leap.Landing")))
	{
		UE_LOG(Enemy, Log, TEXT("Event called"));
		GetActorInfo().AbilitySystemComponent->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag(FName("GameplayCue.Enemy.Leap.Landing")));

		TWeakObjectPtr<AEnemyBase> EnemyBase = Cast<AEnemyBase>(GetActorInfo().AvatarActor);
		if (EnemyBase.IsValid())
		{
			FVector			ImpactPoint = EnemyBase->GetActorLocation() - FVector(0.f, 0.f, EnemyBase->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
			FCollisionShape CollisionShape;
			CollisionShape.SetSphere(ImpactRange);

			TArray<FOverlapResult> OverlapResults;
			GetWorld()->OverlapMultiByChannel(OverlapResults, ImpactPoint, FQuat(), ECollisionChannel::ECC_GameTraceChannel1, CollisionShape);

			for (FOverlapResult OverlapResult : OverlapResults)
			{
				if (ACharacterBase* Player = Cast<ACharacterBase>(OverlapResult.GetActor()))
				{
					if (Player->IsInvincible())
					{
						continue;
					}

					if (ALightSeekerPlayerState* PS = Cast<ALightSeekerPlayerState>(Player->GetPlayerState()))
					{
						FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageGameplayEffect, GetAbilityLevel());
						EnemyBase->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*DamageEffectSpecHandle.Data.Get(), PS->GetAbilitySystemComponent());

						// Apply Knockback to player
						Player->GetMovementComponent()->AddRadialImpulse(ImpactPoint, ImpactRange, MaxImpactForce, ERadialImpulseFalloff::RIF_Linear, true);
					}
				}
			}

			if (DrawAttackRange)
			{
				DrawDebugSphere(GetWorld(), ImpactPoint, ImpactRange, 32, FColor::Blue, false, 2.0f);
			}
		}
	}
}