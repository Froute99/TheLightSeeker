// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilitySystem/GA_CharacterDefaultAttack.h"
#include "Actors/Characters/LightSeekerPlayerState.h"

#include "Actors/Enemies/Abilities/AT_PlayMontageAndWaitForEvent.h"
#include "Characters/CharacterBase.h"
#include "ProjectileBase.h"

void UGA_CharacterDefaultAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//UE_LOG(LogTemp, Log, TEXT("ActivateAbility"));
	//if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	//{
	//	return;
	//}

	////Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);]

	//if (MontageToPlay.IsNull())
	//{
	//	UE_LOG(LogTemp, Error, TEXT("%s() MontageToPlay not set in %s."), *FString(__FUNCTION__), *GetName());
	//	return;
	//}

	//UAT_PlayMontageAndWaitForEvent* Task = UAT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, NAME_None, MontageToPlay, nullptr, FGameplayTagContainer(), 1.0f, NAME_None, false, 1.0f);
	//Task->EventReceived.AddDynamic(this, &UGA_CharacterDefaultAttack::EventReceived);
	//Task->ReadyForActivation();

	ACharacterBase* Player = Cast<ACharacterBase>(Cast<ALightSeekerPlayerState>(GetActorInfo().OwnerActor.Get())->GetPawn());
	FActorSpawnParameters Parameter{};
	Parameter.Instigator = Player;


	Player->GetMesh()->PlayAnimation(MontageToPlay, false);

	FVector Location = Player->GetActorLocation();
	FVector LocationOffset{ 0,0,30.f };

	FRotator Rotation = Player->GetActorRotation();
	FRotator RotationOffset{ 0,90.f,0 };

	AProjectileBase* Arrow = GetWorld()->SpawnActor<AProjectileBase>(ArrowActor, Location + LocationOffset, Rotation, Parameter);

	if (Arrow)
	{
		Arrow->DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageGameplayEffect, GetAbilityLevel());
		FVector LaunchDirection = Player->GetActorForwardVector();
		Arrow->FireInDirection(LaunchDirection);
	}

	// DEBUG
	//UKismetSystemLibrary::DrawDebugLine(GetWorld(), EnemyBase->GetActorLocation(),
	//	PlayerLocation, FLinearColor::Blue, 5.0f);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);







}

void UGA_CharacterDefaultAttack::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	//UE_LOG(Enemy, Log, TEXT("EventReceived called: %s"), EventTag.GetTagName());

	// Montage told us to end the ability before the montage finished playing.
	// Montage was set to continue playing animation even after ability ends so this is okay.
	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.Player.EndAbility")))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		//SetAbilityDoneDelegateHandle.Broadcast();
		return;
	}

	if (!IsValid(DamageGameplayEffect))
	{
		UE_LOG(LogTemp, Error, TEXT("%s() DamageGameplayEffect not set in %s."), *FString(__FUNCTION__), *GetName());
		return;
	}

	ACharacterBase* Player = Cast<ACharacterBase>(Cast<ALightSeekerPlayerState>(GetActorInfo().OwnerActor.Get())->GetPawn());
	//ACharacterBase* Player = Cast<ACharacterBase>(GetActorInfo().OwnerActor.Get());
	//ACharacterBase* Player = Cast<ACharacterBase>(GetOwningActorFromActorInfo());
	if (!Player)
	{
		UE_LOG(LogTemp, Log, TEXT("No player"));
		return;
	}

	// Only spawn projectiles on the Server.
	// Predicting projectiles is an advanced topic not covered in this example.
	if (GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority
		&& EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.Player.DefaultAttack")))
	{
		//AEnemyBase* EnemyBase = Cast<AEnemyBase>(GetActorInfo().OwnerActor.Get());
		UE_LOG(LogTemp, Log, TEXT("Event Received"));

		FActorSpawnParameters Parameter{};
		Parameter.Instigator = Player;


		//GetMesh()->PlayAnimation(AttackMontage, false);

		FVector Location = Player->GetActorLocation();
		FVector LocationOffset{ 0,0,30.f };

		FRotator Rotation = Player->GetActorRotation();
		FRotator RotationOffset{ 0,90.f,0 };

		AProjectileBase* Arrow = GetWorld()->SpawnActor<AProjectileBase>(ArrowActor, Location + LocationOffset, Rotation, Parameter);

		if (Arrow)
		{
			Arrow->DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageGameplayEffect, GetAbilityLevel());
			FVector LaunchDirection = Player->GetActorForwardVector();
			Arrow->FireInDirection(LaunchDirection);
		}

		// DEBUG
		//UKismetSystemLibrary::DrawDebugLine(GetWorld(), EnemyBase->GetActorLocation(),
		//	PlayerLocation, FLinearColor::Blue, 5.0f);
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

}
