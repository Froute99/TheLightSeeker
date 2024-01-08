// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilitySystem/GA_LightningEnchantSpread.h"
#include "GA_LightningEnchantSpread.h"
#include "AbilitySystemComponent.h"
#include "EnemyBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Function for shuffling Enemy Index for random lightning spread effect
void Shuffle(TArray<AEnemyBase*>& Array, int min, int max)
{
	if (min >= max) return;

	srand(time(NULL));

	for (int i = min; i <= max; ++i)
	{
		int Index = min + (FMath::Rand() % (max - min + 1));
		if (Index != i)
		{
			Array.Swap(Index, i);
		}
	}
}

void UGA_LightningEnchantSpread::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (ActivateAbilityOnGranted)
	{
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}

void UGA_LightningEnchantSpread::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	UE_LOG(LogTemp, Warning, TEXT("Activated UGA_LightningEnchantSpread"));
	if (DamageGameplayEffect)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyBase::StaticClass(), FoundActors);

		// Find enemies in range
		TArray<AEnemyBase*> TargetEnemies;
		TargetEnemies.Push(Cast<AEnemyBase>(ActorInfo->AvatarActor.Get()));

		for (AActor* Actor : FoundActors)
		{
			if (Actor == ActorInfo->AvatarActor.Get()) continue;
			if (Actor->IsHidden() == true) continue;

			float Distance = Actor->GetDistanceTo(ActorInfo->AvatarActor.Get());
			if (Distance < SpreadRange)
			{
				TargetEnemies.Push(Cast<AEnemyBase>(Actor));
			}
		}
		Shuffle(TargetEnemies, 1, TargetEnemies.Num() - 1);
		int TargetNum = std::min(TargetEnemies.Num(), MaxTargetNum);
		
		FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageGameplayEffect, GetAbilityLevel());
		// 스킬에 맞은 Enemy: SpreadHolder에서 GameplayEffect 적용함(당사자에겐 적용이 안되는 문제 있음)
		// 
		//ActorInfo->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		//TargetEnemies[0]->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data);

		UKismetSystemLibrary::DrawDebugCapsule(GetWorld(), TargetEnemies[0]->GetActorLocation(), 100.0f, 100.0f, FRotator(), FLinearColor::Red, 3.0f);

		for (int i = 1; i < TargetNum; ++i)
		{
			TargetEnemies[0]->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*DamageEffectSpecHandle.Data.Get(), TargetEnemies[i]->GetAbilitySystemComponent());

			// Chain Effect
			if (TargetEnemies[i]->Children.Num() > 0)
			{

			}

			// Debug Drawing
			UKismetSystemLibrary::DrawDebugCapsule(GetWorld(), TargetEnemies[i]->GetActorLocation(), 100.0f, 100.0f, FRotator(), FLinearColor::Red, 3.0f);
			UKismetSystemLibrary::DrawDebugLine(GetWorld(), TargetEnemies[i - 1]->GetActorLocation(), TargetEnemies[i]->GetActorLocation(), FLinearColor::Red, 3.0f);
		}
	}
}