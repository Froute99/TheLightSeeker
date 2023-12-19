// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilitySystem/GA_SpawnBombAbility.h"
#include "Actors/Characters/CharacterBase.h"
#include "Actors/Characters/LightSeekerPlayerState.h"
#include "Actors/Items/Bomb.h"

void UGA_SpawnBombAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ACharacterBase* Player = Cast<ACharacterBase>(Cast<ALightSeekerPlayerState>(GetActorInfo().OwnerActor.Get())->GetPawn());

	FActorSpawnParameters Parameter{};
	Parameter.Instigator = Player;

	FVector Location = Player->GetActorLocation();
	FVector LocationOffset{ 0.0f, 0.0f, -50.f };

	FTransform Transform;
	Transform.SetLocation(Location + LocationOffset);

	if (BombActor)
	{
		ABomb* Bomb = GetWorld()->SpawnActorDeferred<ABomb>(BombActor, Transform, Player,
			Player, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		Bomb->BombGameplayEffectSpecHandle = MakeOutgoingGameplayEffectSpec(BombGameplayEffect);
		Bomb->FinishSpawning(Transform);

		Bomb->OnSet();
	}
}

void UGA_SpawnBombAbility::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
}

void UGA_SpawnBombAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
}

void UGA_SpawnBombAbility::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	// TODO: combine with animation montage
	
}
