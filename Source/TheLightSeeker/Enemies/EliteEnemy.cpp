// Fill out your copyright notice in the Description page of Project Settings.


#include "EliteEnemy.h"
#include "GameAbilitySystem/CharacterAbilitySystemComponent.h"
#include "GameAbilitySystem/CharacterAttributeSet.h"
#include "UI/EnemyHPBarWidget.h"
#include "Components/WidgetComponent.h"

AEliteEnemy::AEliteEnemy()
{
	HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HP Display Bar"));
	HPBar->SetupAttachment(RootComponent);
}

void AEliteEnemy::BeginPlay()
{
	Super::BeginPlay();

	HealthChangedDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AEliteEnemy::OnHealthChanged);

	UEnemyHPBarWidget* HPBarWidget = Cast <UEnemyHPBarWidget>(HPBar->GetWidgetClass().GetDefaultObject());
	if (HPBarWidget)
	{
		UE_LOG(Enemy, Log, TEXT("HPBarWidget set maxhealth"));
		HPBarWidget->SetMaxHealth(AttributeSet->GetMaxHealth());
	}
	else
	{
		UE_LOG(Enemy, Error, TEXT("HPBarWidget not connected"));
	}
}

void AEliteEnemy::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	Super::OnHealthChanged(Data);

	UEnemyHPBarWidget* HPBarWidget = Cast<UEnemyHPBarWidget>(HPBar->GetWidgetClass().GetDefaultObject());
	if (HPBarWidget)
	{
		UE_LOG(Enemy, Log, TEXT("HPBarWidget set current health"));
		HPBarWidget->SetHealth(Data.NewValue);
	}
	else
	{
		UE_LOG(Enemy, Error, TEXT("HPBarWidget not connected"));
	}
}
