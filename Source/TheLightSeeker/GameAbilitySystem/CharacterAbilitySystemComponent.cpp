// Fill out your copyright notice in the Description page of Project Settings.

#include "GameAbilitySystem/CharacterAbilitySystemComponent.h"
#include "CharacterBase.h"
#include "PlayerHUD.h"

void UCharacterAbilitySystemComponent::ReceiveDamage(float DamageReceived)
{
	ReceivedDamage.Broadcast(DamageReceived);
}

void UCharacterAbilitySystemComponent::LocalInputConfirm()
{
	Super::LocalInputConfirm();
	UE_LOG(LogTemp, Log, TEXT("Input Confirmed"));

	ACharacterBase* Character = Cast<ACharacterBase>(GetAvatarActor());
	Character->IsDoingTargeting = false;
}

void UCharacterAbilitySystemComponent::LocalInputCancel()
{
	Super::LocalInputCancel();
	UE_LOG(LogTemp, Log, TEXT("Input Cancelled"));

	ACharacterBase* Character = Cast<ACharacterBase>(GetAvatarActor());
	Character->IsDoingTargeting = false;
}
