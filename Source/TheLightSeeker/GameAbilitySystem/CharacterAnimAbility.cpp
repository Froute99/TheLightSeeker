// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilitySystem/CharacterAnimAbility.h"
#include "Actors/Characters/LightSeekerPlayerState.h"

#include "Actors/Enemies/Abilities/AT_PlayMontageAndWaitForEvent.h"
#include "Characters/CharacterBase.h"
#include "ProjectileBase.h"
#include "Kismet/KismetMathLibrary.h"

UCharacterAnimAbility::UCharacterAnimAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

}

void UCharacterAnimAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	//UAnimMontage* MontageToPlay = FireHipMontage;

	//if (GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.AimDownSights"))) &&
	//	!GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.AimDownSights.Removal"))))
	//{
	//	MontageToPlay = FireIronsightsMontage;
	//}


	UAT_PlayMontageAndWaitForEvent* Task = UAT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, NAME_None, MontageToPlay, nullptr, FGameplayTagContainer(), 1.0f, NAME_None, false, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &UCharacterAnimAbility::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &UCharacterAnimAbility::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &UCharacterAnimAbility::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &UCharacterAnimAbility::OnCancelled);
	Task->EventReceived.AddDynamic(this, &UCharacterAnimAbility::EventReceived);
	// ReadyForActivation() is how you activate the AbilityTask in C++. Blueprint has magic from K2Node_LatentGameplayTaskCall that will automatically call ReadyForActivation().
	TaskHandle = Task;

	//Task->ReadyForActivation();


	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UCharacterAnimAbility::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UCharacterAnimAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UCharacterAnimAbility::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	// Montage told us to end the ability before the montage finished playing.
	// Montage was set to continue playing animation even after ability ends so this is okay.
	if (EventTag == AnimDoneTag)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		//SetAbilityDoneDelegateHandle.Broadcast();
		return;
	}

	//if (!IsValid(DamageGameplayEffect))
	//{
	//	UE_LOG(LogTemp, Error, TEXT("%s() DamageGameplayEffect not set in %s."), *FString(__FUNCTION__), *GetName());
	//	return;
	//}

	ACharacterBase* Player = Cast<ACharacterBase>(Cast<ALightSeekerPlayerState>(GetActorInfo().OwnerActor.Get())->GetPawn());

	// Only spawn projectiles on the Server.
	// Predicting projectiles is an advanced topic not covered in this example.
	if (GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority
		&& EventTag == AnimTriggerTag)
	{
		UE_LOG(LogTemp, Log, TEXT("Event Received"));

		FActorSpawnParameters Parameter{};
		Parameter.Instigator = Player;

		FVector Location = Player->GetActorLocation();
		FVector LocationOffset{ 0,0,30.f };

		FRotator Rotation = Player->GetActorRotation();
		FRotator RotationOffset{ 0,90.f,0 };


		//FVector Start = Player->GetGunComponent()->GetSocketLocation(FName("Muzzle"));
		//FVector End = Player->GetCameraBoom()->GetComponentLocation() + Player->GetFollowCamera()->GetForwardVector() * 1000.0f;
		//FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(Start, End);

		FTransform Transform;
		Transform.SetLocation(Location);
		Transform.SetRotation(Rotation.Quaternion());
		Transform.SetScale3D(FVector(1.0f));

		//AProjectileBase* Arrow = GetWorld()->SpawnActor<AProjectileBase>(ArrowClass, Location + LocationOffset, Rotation, Parameter);
		AProjectileBase* Projectile = GetWorld()->SpawnActorDeferred<AProjectileBase>(ArrowClass, Transform, Player,
			Player, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageGameplayEffect);

		Projectile->DamageEffectSpecHandle = DamageEffectSpecHandle;
		//Projectile->Range = Range;
		Projectile->FinishSpawning(Transform);
	}

}

void UCharacterAnimAbility::PlayAnim()
{
	TaskHandle->ReadyForActivation();

}
