// Copyright (c) 2023 Team Light Seekers All rights reserved.


#include "UI/PlayerHUD.h"
#include "LightSeekerPlayerState.h"
#include "CharacterAbilitySystemComponent.h"

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

}

void APlayerHUD::BindDelegate()
{
	ALightSeekerPlayerState* PS = Cast<APlayerController>(GetOwner())->GetPlayerState<ALightSeekerPlayerState>();

	UCharacterAbilitySystemComponent* ASC = Cast<UCharacterAbilitySystemComponent>(PS->GetAbilitySystemComponent());

	ASC->ReceivedDamage.AddDynamic(this, &APlayerHUD::SpawnFloatingDamageTextWidget);
}
