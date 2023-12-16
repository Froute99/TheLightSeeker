// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Enemies/EnemyBase.h"
#include "GameAbilitySystem/CharacterAttributeSet.h"
#include "GameAbilitySystem/CharacterAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Actors/Items/Item.h"

AEnemyBase::AEnemyBase()
{
	ASC = CreateDefaultSubobject<UCharacterAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("AttributeSet"));

	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetCapsuleComponent());
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
		UE_LOG(Enemy, Log, TEXT("Enemy ASC initialized"));
		ASC->InitAbilityActorInfo(this, this);
		InitializeAttributes();
		AddStartupEffects();
		AddCharacterAbilities();

		// attribute change handles
		HealthChangedDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AEnemyBase::OnHealthChanged);
		MoveSpeedChangedDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMovementSpeedAttribute()).AddUObject(this, &AEnemyBase::OnMoveSpeedChanged);
		// tag change callbacks

		IsDying = false;

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

void AEnemyBase::OnDied()
{
	UE_LOG(Enemy, Log, TEXT("OnDied Called"));
	RemoveCharacterAbilities();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector(0);

	IsDying = true;
	
	auto& Effects = ASC->GetActiveGameplayEffects();

	for (auto it = Effects.CreateConstIterator(); it; ++it)
	{
		ASC->RemoveActiveGameplayEffect(it->Handle);
	}

	ASC->RemoveAllGameplayCues();

	//OnCharacterDied.Broadcast(this);

	if (ASC)
	{
		ASC->CancelAllAbilities();

		//FGameplayTagContainer EffectTagsToRemove;
		//EffectTagsToRemove.AddTag(EffectRemoveOnDeathTag);
		//int32 NumEffectsRemoved = AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectTagsToRemove);
		//
		//AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
	}

	if (DeathAnimMontage)
	{
		PlayAnimMontage(DeathAnimMontage);
		if (WeaponDeathAnimMontage)
		{
			WeaponMesh->GetAnimInstance()->Montage_Play(WeaponDeathAnimMontage);
		}
	}
	else
	{
		UE_LOG(Enemy, Log, TEXT("Enemy Missing Death Animation Montage!"));
		DropItem();
		Destroy();
	}
}

void AEnemyBase::DropItem()
{
	UE_LOG(Enemy, Log, TEXT("Enemy dropped Item"));
	
	FActorSpawnParameters Parameter{};
	Parameter.Instigator = this;
	FVector LaunchLocation = GetActorLocation();
	AItem* SpawnedItem = GetWorld()->SpawnActor<AItem>(Item, LaunchLocation, FRotator(), Parameter);
}

void AEnemyBase::FinishDying()
{
	UE_LOG(Enemy, Log, TEXT("Enemy FinishDying"));
	Destroy();
}

TWeakObjectPtr<USkeletalMeshComponent> AEnemyBase::GetWeaponMesh() const
{
	return WeaponMesh;
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
	UE_LOG(Enemy, Log, TEXT("Damaged from: %s"), *Name);
}

void AEnemyBase::HandleHealthChanged(float Value, const FGameplayTagContainer& SourceTags)
{
	UE_LOG(Enemy, Log, TEXT("Health Changed"));
}

void AEnemyBase::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	float Health = Data.NewValue;

	// If the minion died, handle death
	if (!IsAlive() && !IsDying)
	{
		UE_LOG(Enemy, Log, TEXT("OnHealthChanged - OnDied"));
		OnDied();
	}
}

void AEnemyBase::OnMoveSpeedChanged(const FOnAttributeChangeData& Data)
{
	float NewSpeed = Data.NewValue;
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
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
			UE_LOG(Enemy, Log, TEXT("SetHealth - OnDied"));
			OnDied();
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
		UE_LOG(Enemy, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = ASC->MakeOutgoingSpec(DefaultAttributes, 1, EffectContext);

	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = ASC->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), ASC);
	}
	
	// Temporary
	SetHealth(AttributeSet->GetMaxHealth());

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


void AEnemyBase::RemoveCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !ASC || !ASC->CharacterAbilitiesGiven)
	{
		return;
	}

	// Remove any abilities added from a previous call. This checks to make sure the ability is in the startup 'CharacterAbilities' array.
	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for (const FGameplayAbilitySpec& Spec : ASC->GetActivatableAbilities())
	{
		if (Spec.SourceObject == this)
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}

	// Do in two passes so the removal happens after we have the full list
	for (int32 i = 0; i < AbilitiesToRemove.Num(); i++)
	{
		ASC->ClearAbility(AbilitiesToRemove[i]);
	}

	ASC->CharacterAbilitiesGiven = false;
}