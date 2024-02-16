// Copyright (c) 2023 Team Light Seekers All rights reserved.


#include "UI/PlayerHUD.h"
#include "LightSeekerPlayerState.h"
#include "CharacterAbilitySystemComponent.h"

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

}

bool APlayerHUD::BindDelegate()
{
	ALightSeekerPlayerState* PS = Cast<ALightSeekerPlayerState>(PlayerOwner->PlayerState);
	if (!IsValid(PS))
	{
		UE_LOG(LogTemp, Warning, TEXT("PS is invalid"));
		return false;
	}

	UCharacterAbilitySystemComponent* ASC = Cast<UCharacterAbilitySystemComponent>(PS->GetAbilitySystemComponent());
	if (!IsValid(ASC))
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot get ASC"));
		return false;
	}

	//ASC->ReceivedDamage.AddDynamic(this, &APlayerHUD::SpawnFloatingDamageTextWidget);
	return true;
}
