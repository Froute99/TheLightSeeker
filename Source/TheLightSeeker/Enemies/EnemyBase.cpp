// Copyright (c) 2023 Team Light Seekers All rights reserved.

#include "EnemyBase.h"
#include "CharacterAttributeSet.h"
#include "CharacterAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Items/Item.h"
#include "AIController.h"
#include "BrainComponent.h"

#include "UI/EnemyHPBarWidget.h"
#include "Components/WidgetComponent.h"

AEnemyBase::AEnemyBase()
{
	ASC = CreateDefaultSubobject<UCharacterAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("AttributeSet"));

	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetCapsuleComponent());

	HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HP Display Bar"));
	HPBar->SetupAttachment(RootComponent);
}

UBehaviorTree* AEnemyBase::GetBTAsset() const
{
	return BTAsset;
}

void AEnemyBase::SetTargetPlayer(APawn* Target)
{
	TargetPlayer = Target;

	GetWorldTimerManager().ClearTimer(TargetResetTimerHandle);
	GetWorldTimerManager().SetTimer(TargetResetTimerHandle, FTimerDelegate::CreateLambda([this]() { TargetPlayer = nullptr; }), 10.0f, false);
}

APawn* AEnemyBase::GetTargetPlayer() const
{
	return TargetPlayer;
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	if (ASC != nullptr)
	{
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
	
	UEnemyHPBarWidget* HPBarWidget = Cast<UEnemyHPBarWidget>(HPBar->GetUserWidgetObject());
	if (HPBarWidget)
	{
		UE_LOG(Enemy, Log, TEXT("HPBarWidget set maxhealth"));
		HPBarWidget->SetMaxHealth(AttributeSet->GetMaxHealth());
	}
	else
	{
		//UE_LOG(Enemy, Error, TEXT("HPBarWidget not connected2"));
	}
}

float AEnemyBase::GetAttackRange() const
{
	return AttackRange;
}

void AEnemyBase::OnDied()
{
	UE_LOG(Enemy, Log, TEXT("OnDied Called"));

	IsDying = true;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector(0);

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

	// Server-side only
	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	// stop enemy behavior tree so enemy does not rotate to facing player
	CastChecked<AAIController>(GetController())->GetBrainComponent()->StopLogic("Enemy Dead");
	RemoveCharacterAbilities();
	
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
}

void AEnemyBase::DropItem()
{
	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (Item)
	{
		UE_LOG(Enemy, Log, TEXT("Enemy dropped Item"));

		FActorSpawnParameters Parameter{};
		Parameter.Instigator = this;
		FVector LaunchLocation = GetActorLocation();
		AItem* SpawnedItem = GetWorld()->SpawnActor<AItem>(Item, LaunchLocation, FRotator(), Parameter);

		if(!SpawnedItem)
		{
			UE_LOG(Enemy, Error, TEXT("Enemy dropped Item: failed to spawn item actor"));
		}
	}
}

void AEnemyBase::FinishDying()
{
	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	UE_LOG(Enemy, Log, TEXT("Enemy FinishDying"));
	Destroy();
}

TWeakObjectPtr<USkeletalMeshComponent> AEnemyBase::GetWeaponMesh() const
{
	return WeaponMesh;
}

float AEnemyBase::GetScale() const
{
	return GetActorScale().X;
}

void AEnemyBase::OnActivate()
{
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
	SetActorHiddenInGame(false);

	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		if (UBrainComponent* BrainComponent = AIController->GetBrainComponent())
		{
			BrainComponent->RestartLogic();
		}
	}
}

void AEnemyBase::OnDeactivate()
{
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	SetActorHiddenInGame(true);

	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		if(UBrainComponent* BrainComponent = AIController->GetBrainComponent())
		{
			BrainComponent->StopLogic(FString(TEXT("Waiting for activation..")));
		}
	}
}

void AEnemyBase::Multicast_PlayWeaponAnimMontage_Implementation(UAnimMontage* Montage)
{
	UAnimInstance* WeaponAnimInstance = GetWeaponMesh()->GetAnimInstance();
	WeaponAnimInstance->Montage_Play(Montage);
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

	UEnemyHPBarWidget* HPBarWidget = Cast<UEnemyHPBarWidget>(HPBar->GetUserWidgetObject());
	if (HPBarWidget)
	{
		UE_LOG(Enemy, Log, TEXT("HPBarWidget set current health"));
		HPBarWidget->SetHealth(Data.NewValue);
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