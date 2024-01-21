// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BossEnemy.h"
#include "GameAbilitySystem/CharacterAbilitySystemComponent.h"
#include "GameAbilitySystem/CharacterAttributeSet.h"

void ABossEnemy::BeginPlay()
{
	Super::BeginPlay();

	HealthChangedDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &ABossEnemy::OnHealthChanged);

	//UEnemyHPBarWidget* HPBarWidget = Cast<UEnemyHPBarWidget>(HPBar->GetUserWidgetObject());
	//if (HPBarWidget)
	//{
	//	UE_LOG(Enemy, Log, TEXT("HPBarWidget set maxhealth"));
	//	HPBarWidget->SetMaxHealth(AttributeSet->GetMaxHealth());
	//}
	//else
	//{
	//	UE_LOG(Enemy, Error, TEXT("HPBarWidget not connected"));
	//}
}

void ABossEnemy::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	Super::OnHealthChanged(Data);

	//UEnemyHPBarWidget* HPBarWidget = Cast<UEnemyHPBarWidget>(HPBar->GetUserWidgetObject());
	//if (HPBarWidget)
	//{
	//	UE_LOG(Enemy, Log, TEXT("HPBarWidget set current health"));
	//	//HPBarWidget->SetHealth(Data.NewValue);	
	//}
	//else
	//{
	//	UE_LOG(Enemy, Error, TEXT("HPBarWidget not connected"));
	//}
}
