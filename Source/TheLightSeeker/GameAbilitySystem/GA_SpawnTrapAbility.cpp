// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilitySystem/GA_SpawnTrapAbility.h"
#include "Actors/Characters/CharacterBase.h"
#include "Actors/Characters/LightSeekerPlayerState.h"
#include "Actors/Trap.h"

void UGA_SpawnTrapAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ACharacterBase* Player = Cast<ACharacterBase>(Cast<ALightSeekerPlayerState>(GetActorInfo().OwnerActor.Get())->GetPawn());

	FActorSpawnParameters Parameter{};
	Parameter.Instigator = Player;

	FVector Location = Player->GetActorLocation();
	FVector LocationOffset{ 0.0f, 0.0f, -50.f };

	FTransform Transform;
	Transform.SetLocation(Location + LocationOffset);

	if (TrapActor)
	{
		ATrap* Trap = GetWorld()->SpawnActorDeferred<ATrap>(TrapActor, Transform, Player,
			Player, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		Trap->TrapGameplayEffectSpecHandle = MakeOutgoingGameplayEffectSpec(TrapGameplayEffect);
		Trap->FinishSpawning(Transform);
	}
}

void UGA_SpawnTrapAbility::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
}

void UGA_SpawnTrapAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
}

void UGA_SpawnTrapAbility::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	// todo : combine with animation montage
}
