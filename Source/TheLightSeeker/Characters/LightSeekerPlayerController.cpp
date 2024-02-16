// Copyright (c) 2023 Team Light Seekers All rights reserved.

#include "Characters/LightSeekerPlayerController.h"
#include "LightSeekerPlayerState.h"
#include "CharacterAbilitySystemComponent.h"

void ALightSeekerPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

bool ALightSeekerPlayerController::BindDelegate()
{
	ALightSeekerPlayerState* PS = Cast<ALightSeekerPlayerState>(PlayerState);
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

	ASC->OnReceivedDamage.AddDynamic(this, &ALightSeekerPlayerController::FloatingDamage);
	return true;
}
