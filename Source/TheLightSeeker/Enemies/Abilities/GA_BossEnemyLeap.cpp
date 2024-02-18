// Copyright (c) 2023 Team Light Seekers All rights reserved.

#include "Enemies/Abilities/GA_BossEnemyLeap.h"
#include "AbilitySystemComponent.h"
#include "AT_PlayMontageAndWaitForEvent.h"
#include "EnemyBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

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

	// if (PlayerReference)
	//{
	//	TargetLocation = PlayerReference->GetActorLocation();
	//	TargetLocation.Z = 0.0f;
	// }

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

	if (GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority
		&& EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.Enemy.Boss.Leap.Start")))
	{
		FVector CurrentLocation = GetOwningActorFromActorInfo()->GetActorLocation();
		CurrentLocation.Z = 0.0f;

		FVector						 LeapVelocity = (TargetLocation - CurrentLocation) / LeapDuration;
		UCharacterMovementComponent* EnemyCharacterMovement = Cast<ACharacter>(GetOwningActorFromActorInfo())->GetCharacterMovement();
		Cast<ACharacter>(GetOwningActorFromActorInfo())->FaceRotation(LeapVelocity.Rotation());
		EnemyCharacterMovement->Velocity = LeapVelocity;
		EnemyCharacterMovement->DoJump(false);
		// UE_LOG(Enemy, Log, TEXT("Tried jump: %i"),  ? 1 : 0);
	}

	if (GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority
		&& EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.Enemy.Boss.Leap.End")))
	{
		UCharacterMovementComponent* EnemyCharacterMovement = Cast<ACharacter>(GetOwningActorFromActorInfo())->GetCharacterMovement();
		EnemyCharacterMovement->Velocity = FVector::ZeroVector;
	}
}
