// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD.h"
#include "LightSeekerPlayerState.h"
#include "CharacterAbilitySystemComponent.h"

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	ALightSeekerPlayerState* PS = Cast<APlayerController>(GetOwner())->GetPlayerState<ALightSeekerPlayerState>();

	UCharacterAbilitySystemComponent* ASC = Cast<UCharacterAbilitySystemComponent>(PS->GetAbilitySystemComponent());

	ASC->ReceivedDamage.AddDynamic(this, &APlayerHUD::SpawnFloatingDamageTextWidget);
}
