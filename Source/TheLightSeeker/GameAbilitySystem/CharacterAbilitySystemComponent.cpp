// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbilitySystem/CharacterAbilitySystemComponent.h"


void UCharacterAbilitySystemComponent::ReceiveDamage(UCharacterAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage)
{
	ReceivedDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
	
	UE_LOG(LogTemp, Log, TEXT("Received Damage"));
}

