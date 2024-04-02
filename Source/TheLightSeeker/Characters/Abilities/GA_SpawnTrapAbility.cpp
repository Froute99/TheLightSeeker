// Copyright (c) 2023 Team Light Seekers All rights reserved.

#include "GA_SpawnTrapAbility.h"
#include "Characters/CharacterBase.h"
#include "Characters/LightSeekerPlayerState.h"
#include "Items/Trap.h"
#include "Kismet/GameplayStatics.h"

void UGA_SpawnTrapAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ACharacterBase* Player = Cast<ACharacterBase>(Cast<ALightSeekerPlayerState>(GetActorInfo().OwnerActor.Get())->GetPawn());

	FActorSpawnParameters Parameter{};
	Parameter.Instigator = Player;

	FVector Location = Player->GetActorLocation();
	FVector LocationOffset{ 0.0f, 0.0f, -80.f };

	FTransform Transform;
	Transform.SetLocation(Location + LocationOffset);

	if (TrapActor)
	{
		ATrap* Trap = GetWorld()->SpawnActorDeferred<ATrap>(TrapActor, Transform, Player,
			Player, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		Trap->TrapGameplayEffectSpecHandle = MakeOutgoingGameplayEffectSpec(TrapGameplayEffect);
		Trap->FinishSpawning(Transform);

		UGameplayStatics::PlaySound2D(GetWorld(), SpawnSound);
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
	// TODO: combine with animation montage
}
