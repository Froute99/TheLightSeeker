// Fill out your copyright notice in the Description page of Project Settings.

#include "GameAbilitySystem/CharacterAbilitySystemComponent.h"
#include "CharacterBase.h"
#include "PlayerHUD.h"

void UCharacterAbilitySystemComponent::ReceiveDamage(float DamageReceived)
{
	//UE_LOG(LogTemp, Log, TEXT("ASC ReceivedDamage Called"));
	ReceivedDamage.Broadcast(DamageReceived);
}

void UCharacterAbilitySystemComponent::LocalInputConfirm()
{
	Super::LocalInputConfirm();

	ACharacterBase* Character = Cast<ACharacterBase>(GetAvatarActor());
	Character->IsDoingTargeting = false;
}

void UCharacterAbilitySystemComponent::LocalInputCancel()
{
	Super::LocalInputCancel();

	ACharacterBase* Character = Cast<ACharacterBase>(GetAvatarActor());
	Character->IsDoingTargeting = false;
}
