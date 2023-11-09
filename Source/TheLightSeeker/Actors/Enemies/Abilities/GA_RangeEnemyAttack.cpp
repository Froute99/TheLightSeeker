// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Enemies/Abilities/GA_RangeEnemyAttack.h"
#include "AbilitySystemComponent.h"
#include "Actors/Enemies/Abilities/AT_PlayMontageAndWaitForEvent.h"
#include "Actors/Enemies/EnemyBase.h"
#include "Actors/Characters/LightSeekerPlayerState.h"
#include "Components/CapsuleComponent.h"
#include "Actors/Enemies/Abilities/RangeEnemyProjectile.h"
#include "Kismet/KismetSystemLibrary.h"

void UGA_RangeEnemyAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	UE_LOG(Enemy, Log, TEXT("RangeEnemyAttack called"));

	if (!AttackMontage)
	{
		UE_LOG(Enemy, Error, TEXT("RangeEnemyAttack class does not have Montage to play"))
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	UAnimMontage* MontageToPlay = AttackMontage;

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

void UGA_RangeEnemyAttack::SetPlayerReference(AActor* Player)
{
	if (!Cast<ACharacterBase>(Player))
	{
		UE_LOG(Enemy, Error, TEXT("SetPlayerReference: given player is not a player actor"));
		return;
	}
	PlayerReference = Player;
}

void UGA_RangeEnemyAttack::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	//UE_LOG(Enemy, Log, TEXT("EventReceived called: %s"), EventTag.GetTagName());

	// Montage told us to end the ability before the montage finished playing.
	// Montage was set to continue playing animation even after ability ends so this is okay.
	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.EndAbility")))
	{
		OnCompleted(EventTag, EventData);
		return;
	}

	// Only spawn projectiles on the Server.
	// Predicting projectiles is an advanced topic not covered in this example.
	if (GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority
		&& EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.Enemy.RangeAttack")))
	{
		AEnemyBase* EnemyBase = Cast<AEnemyBase>(GetActorInfo().OwnerActor.Get());

		if (EnemyBase && PlayerReference)
		{
			FVector PlayerLocation = PlayerReference->GetActorLocation();

			FActorSpawnParameters Parameter{};
			Parameter.Instigator = EnemyBase;
			FVector LaunchLocation = EnemyBase->GetActorLocation() + FVector(0.0f, 0.0f, EnemyBase->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
			ARangeEnemyProjectile* Projectile = GetWorld()->SpawnActor<ARangeEnemyProjectile>(ProjectileActor, 
				    LaunchLocation,
					FRotator(), Parameter);

			if (Projectile)
			{
				Projectile->DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageGameplayEffect, GetAbilityLevel());
				FVector LaunchDirection = PlayerLocation - LaunchLocation;
				Projectile->FireInDirection(LaunchDirection.GetSafeNormal());
			}

			// DEBUG 
			UKismetSystemLibrary::DrawDebugLine(GetWorld(), EnemyBase->GetActorLocation(),
				PlayerLocation, FLinearColor::Blue, 5.0f);
		}
	}
}