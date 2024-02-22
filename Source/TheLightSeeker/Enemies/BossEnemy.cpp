// Copyright (c) 2023 Team Light Seekers All rights reserved.

#include "Enemies/BossEnemy.h"
#include "GameAbilitySystem/CharacterAbilitySystemComponent.h"
#include "GameAbilitySystem/CharacterAttributeSet.h"
#include "UI/EnemyHPBarWidget.h"
#include "GameFramework/GameState.h"
#include "Characters/LightSeekerPlayerState.h"

void ABossEnemy::BeginPlay()
{
	Super::BeginPlay();

	HealthChangedDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &ABossEnemy::OnHealthChanged);
}

void ABossEnemy::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	Super::OnHealthChanged(Data);

	if(GetLocalRole() != ROLE_Authority)
	{
		return;
	}
	UE_LOG(Enemy, Warning, TEXT("Boss Max, Health: %f, %f"), AttributeSet->GetMaxHealth(), AttributeSet->GetHealth());
	for (auto PS : PlayerStates)
	{
		PS->RepBossHealthBar(Data.NewValue, false);
	}
}

void ABossEnemy::OnActivate()
{
	Super::OnActivate();

	TArray<TObjectPtr<APlayerState>> Players;
	int								 TotalPlayerNum = GetWorld()->GetGameState()->PlayerArray.Num();

	if (TotalPlayerNum <= 0)
	{
		return;
	}

	PlayerStates.Reset();
	for (int i = 0; i < TotalPlayerNum; ++i)
	{
		ALightSeekerPlayerState* PS = Cast<ALightSeekerPlayerState>(GetWorld()->GetGameState()->PlayerArray[i]);
		if (PS)
		{
			UE_LOG(Enemy, Warning, TEXT("ABossEnemy::HPBar Init with hp %i"), AttributeSet->GetMaxHealth());
			PS->RepBossHealthBar(AttributeSet->GetMaxHealth(), true);
			PlayerStates.Add(PS);
		}
	}

	UE_LOG(Enemy, Warning, TEXT("ABossEnemy::OnActivate PS num: %i"), PlayerStates.Num());
}

void ABossEnemy::OnDeactivate()
{
	Super::OnDeactivate();
}

void ABossEnemy::InitializeHealthBar()
{
	/*if (HPBar)
	{
		HPBar->SetVisibility(ESlateVisibility::Hidden);
		HPBar->SetMaxHealth(AttributeSet->GetMaxHealth());
	}*/
}
