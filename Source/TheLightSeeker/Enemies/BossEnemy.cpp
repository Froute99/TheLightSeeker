// Copyright (c) 2023 Team Light Seekers All rights reserved.

#include "Enemies/BossEnemy.h"
#include "GameAbilitySystem/CharacterAbilitySystemComponent.h"
#include "GameAbilitySystem/CharacterAttributeSet.h"
#include "UI/EnemyHPBarWidget.h"

void ABossEnemy::BeginPlay()
{
	Super::BeginPlay();

	HealthChangedDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &ABossEnemy::OnHealthChanged);
}

void ABossEnemy::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	Super::OnHealthChanged(Data);

	if (HPBar)
	{
		UE_LOG(Enemy, Log, TEXT("HPBarWidget set current health"));
		HPBar->SetHealth(Data.NewValue);
	}
	else
	{
		UE_LOG(Enemy, Error, TEXT("HPBarWidget not connected"));
	}
}

void ABossEnemy::OnActivate()
{
	Super::OnActivate();

	if (HPBar)
	{
		UE_LOG(Enemy, Log, TEXT("HPBarWidget set maxhealth"));
		HPBar->SetVisibility(ESlateVisibility::Visible);
		HPBar->SetMaxHealth(AttributeSet->GetMaxHealth());
	}
	else
	{
		UE_LOG(Enemy, Error, TEXT("OnActivate: Boss HPBarWidget not connected"));
	}
}

void ABossEnemy::OnDeactivate()
{
	Super::OnDeactivate();

	if (HPBar)
	{
		HPBar->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		UE_LOG(Enemy, Error, TEXT("OnDeactivate: Boss HPBarWidget not connected"));
	}
}

void ABossEnemy::InitializeHealthBar()
{
	if (HPBar)
	{
		HPBar->SetVisibility(ESlateVisibility::Hidden);
		HPBar->SetMaxHealth(AttributeSet->GetMaxHealth());
	}
}
