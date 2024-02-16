// Fill out your copyright notice in the Description page of Project Settings.

#include "LightSeekerPlayerState.h"
#include "CharacterAttributeSet.h"
#include "CharacterAbilitySystemComponent.h"
#include "CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/PlayerHealthBarWidget.h"
#include "PlayerHUD.h"

#include "Kismet/KismetSystemLibrary.h"

ALightSeekerPlayerState::ALightSeekerPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UCharacterAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("AttributeSet"));

	NetUpdateFrequency = 100.0f;

	ElementalEffectDone.BindUObject(this, &ALightSeekerPlayerState::ClearElementalEffect);
}

UAbilitySystemComponent* ALightSeekerPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UCharacterAttributeSet* ALightSeekerPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

bool ALightSeekerPlayerState::IsAlive() const
{
	return GetHealth() > 0.0f;
}

float ALightSeekerPlayerState::GetHealth() const
{
	return AttributeSet->GetHealth();
}

float ALightSeekerPlayerState::GetMaxHealth() const
{
	return AttributeSet->GetMaxHealth();
}

int32 ALightSeekerPlayerState::GetCharacterLevel() const
{
	return FMath::Floor(AttributeSet->GetLevel());
}

void ALightSeekerPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &ALightSeekerPlayerState::HealthChanged);
		MaxHealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHealthAttribute()).AddUObject(this, &ALightSeekerPlayerState::MaxHealthChanged);
		LevelChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetLevelAttribute()).AddUObject(this, &ALightSeekerPlayerState::LevelChanged);
		MoveSpeedChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMovementSpeedAttribute()).AddUObject(this, &ALightSeekerPlayerState::MoveSpeedChanged);
	}
}

void ALightSeekerPlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Log, TEXT("%s : Player Health Changed"), *FString(__FUNCTION__));

	//UPlayerHealthBarWidget* HealthBar = nullptr;
	//HealthBar = Cast<ACharacterBase>(GetPlayerController()->GetPawn())->HealthBar;
	RepHealthBar(Data.NewValue);

	// if (IsValid(HealthBar))
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Data.NewValue);

	//	//HealthBar->SetHealth(std::min(Data.NewValue, GetMaxHealth()));

	//	RepHealthBar(Data.NewValue);

	//	if (FMath::IsNearlyZero(Data.NewValue))
	//	{
	//		// TODO: Add "Die" functions here
	//		UE_LOG(LogTemp, Log, TEXT("Character Dead"));
	//	}
	//}
	// else
	//{
	//	UE_LOG(LogTemp, Log, TEXT("HealthBar invalid"));
	//}
}

void ALightSeekerPlayerState::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Log, TEXT("%s : Player Max Health Changed"), *FString(__FUNCTION__));
}

void ALightSeekerPlayerState::LevelChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("%s : Player Level Changed!"), *FString(__FUNCTION__));
}

void ALightSeekerPlayerState::MoveSpeedChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("%s : Player MoveSpeed Changed!"), *FString(__FUNCTION__))

	ACharacterBase* Character = Cast<ACharacterBase>(GetPlayerController()->GetPawn());
	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = AttributeSet->GetMovementSpeed();
	}
}

void ALightSeekerPlayerState::RepHealthBar_Implementation(float NewValue)
{
	UE_LOG(LogTemp, Log, TEXT("Replicate HealthBar"));

	APlayerController* PC = GetPlayerController();
	if (!IsValid(PC))
	{
		UE_LOG(LogTemp, Log, TEXT("PC invalid"));
		return;
	}

	if (!PC->IsLocalController())
	{
		UE_LOG(LogTemp, Log, TEXT("Not Local Controller"));
		return;
	}

	APawn* Pawn = PC->GetPawn();
	if (!IsValid(Pawn))
	{
		UE_LOG(LogTemp, Log, TEXT("Pawn invalid"));
		return;
	}

	ACharacterBase* Character = Cast<ACharacterBase>(Pawn);
	Character->UpdateHealthBar();
}

void ALightSeekerPlayerState::ClearElementalEffect()
{
	UE_LOG(LogTemp, Log, TEXT("ClearElemenetalEffect called"));
	ElementalEffectHandle = nullptr;
}
