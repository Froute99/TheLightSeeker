// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Enemies/EnemyBase.h"
#include "GameAbilitySystem/CharacterAttributeSet.h"
#include "GameAbilitySystem/CharacterAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Actors/Characters/LightSeekerPlayerState.h"

AEnemyBase::AEnemyBase()
{
	ASC = CreateDefaultSubobject<UCharacterAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("AttributeSet"));

	PrimaryActorTick.bCanEverTick = true;
}

UBehaviorTree* AEnemyBase::GetBTAsset() const
{
	return BTAsset;
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	if (ASC != nullptr)
	{
		//UE_LOG(Enemy, Log, TEXT("Enemy ASC initialized"));
		ASC->InitAbilityActorInfo(this, this);
		InitializeAttributes();
		AddStartupEffects();
		AddCharacterAbilities();

		// health change handle (for future interface)
		HealthChangedDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AEnemyBase::OnHealthChanged);
		// tag change callbacks


		UE_LOG(Enemy, Log, TEXT("After init - Health: %f"), GetHealth());
		UE_LOG(Enemy, Log, TEXT("After init - MaxHealth: %f"), GetMaxHealth());
		UE_LOG(Enemy, Log, TEXT("After init - Speed: %f"), AttributeSet->GetMovementSpeed());
	}
	else
	{
		UE_LOG(Enemy, Log, TEXT("Enemy ASC Init could not called"));
	}
}

float AEnemyBase::GetAttackRange() const
{
	return AttackRange;
}

void AEnemyBase::OnDeath()
{
	UE_LOG(Enemy, Log, TEXT("Enemy OnDeath Called"));
	Destroy();

	/*
	// Only runs on Server
	RemoveCharacterAbilities();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector(0);

	OnCharacterDied.Broadcast(this);

	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->CancelAllAbilities();

		FGameplayTagContainer EffectTagsToRemove;
		EffectTagsToRemove.AddTag(EffectRemoveOnDeathTag);
		int32 NumEffectsRemoved = AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectTagsToRemove);

		AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
	}

	if (DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
	}
	else
	{
		FinishDying();
	} */
}

UAbilitySystemComponent* AEnemyBase::GetAbilitySystemComponent() const
{
	return ASC;
}

UCharacterAttributeSet* AEnemyBase::GetAttributeSet() const
{
	return AttributeSet;
}

void AEnemyBase::HandleDamage(float Damage, const FHitResult& HitResult, const FGameplayTagContainer& SourceTags, ACharacterBase* SourceCharacter, AActor* SourceActor)
{
	FString Name = SourceCharacter->GetName();
	UE_LOG(LogTemp, Log, TEXT("Damaged from: %s"), *Name);
}

void AEnemyBase::HandleHealthChanged(float Value, const FGameplayTagContainer& SourceTags)
{
	UE_LOG(LogTemp, Log, TEXT("Health Changed"));
}

void AEnemyBase::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	float Health = Data.NewValue;

	// If the minion died, handle death
	if (!IsAlive())
	{
		OnDeath();
	}
}

float AEnemyBase::GetHealth() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetHealth();
	}
	return 0.0f;
}

float AEnemyBase::GetMaxHealth() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetMaxHealth();
	}
	return 0.0f;
}

void AEnemyBase::SetHealth(float Value)
{
	if (AttributeSet)
	{
		AttributeSet->SetHealth(Value);
		if (Value <= 0.0f)
		{
			OnDeath();
		}
	}
}

void AEnemyBase::SetMaxHealth(float Value)
{
	if (AttributeSet)
	{
		AttributeSet->SetMaxHealth(Value);
	}
}

void AEnemyBase::AddCharacterAbilities()
{
	// Grant abilities, but only on the server	
	if (GetLocalRole() != ROLE_Authority || ASC == nullptr || ASC->CharacterAbilitiesGiven)
	{
		UE_LOG(Enemy, Log, TEXT("AddCharacterAbilities failed"));
		return;
	}

	for (TSubclassOf<UGameplayAbility>& StartupAbility : EnemyAbilities)
	{
		ASC->GiveAbility(
			FGameplayAbilitySpec(StartupAbility, 1, -1, this));
	}

	ASC->CharacterAbilitiesGiven = true;
}

void AEnemyBase::InitializeAttributes()
{
	if (ASC == nullptr)
	{
		return;
	}

	if (!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = ASC->MakeOutgoingSpec(DefaultAttributes, 1, EffectContext);

	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = ASC->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), ASC);
	}
	
	// set WalkSpeed based on attribute
	GetCharacterMovement()->MaxWalkSpeed = AttributeSet->GetMovementSpeed();
}	
	
void AEnemyBase::AddStartupEffects()
{
	if(GetLocalRole() != ROLE_Authority || ASC == nullptr || ASC->StartupEffectApplied)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = ASC->MakeOutgoingSpec(GameplayEffect, 0.0f, EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = ASC->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), ASC);
		}
	}

	ASC->StartupEffectApplied = true;
}

bool AEnemyBase::IsAlive() const
{
	return GetHealth() > 0.0f;
}
