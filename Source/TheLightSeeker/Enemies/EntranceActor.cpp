// Copyright (c) 2023 Team Light Seekers All rights reserved.

#include "Enemies/EntranceActor.h"
#include "CharacterAttributeSet.h"
#include "CharacterAbilitySystemComponent.h"
#include "UI/EnemyHPBarWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/AudioComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

AEntranceActor::AEntranceActor()
{
	ASC = CreateDefaultSubobject<UCharacterAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("AttributeSet"));

	HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HP Display Bar"));
	HPBar->SetupAttachment(RootComponent);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);

	if (AudioComponent->IsValidLowLevelFast() && HitSound)
	{
		AudioComponent->SetSound(HitSound);
	}

	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	GeometryCollection->SetSimulatePhysics(false);
	GeometryCollection->SetupAttachment(RootComponent);
}

void AEntranceActor::BeginPlay()
{
	Super::BeginPlay();

	if (ASC != nullptr)
	{
		ASC->InitAbilityActorInfo(this, this);
		InitializeAttributes();

		HealthChangedDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AEntranceActor::OnHealthChanged);
	}
}

void AEntranceActor::OnDied()
{
	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (GeometryCollection)
	{
		GeometryCollection->SetSimulatePhysics(true);
		GeometryCollection->AddRadialImpulse(GetActorLocation(), 10.f, 100.f, ERadialImpulseFalloff::RIF_Constant);
		//GeometryCollection->AddImpulse(FVector(10000.f, 0.f, 10000.f));
	}

	GetStaticMeshComponent()->SetHiddenInGame(true);

	// timer for destroy actor
	FTimerHandle SelfDestroyTimerHandle;
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateLambda([this]() {
		if (this)
		{
			this->Destroy();
		}
	});
	GetWorld()->GetTimerManager().SetTimer(SelfDestroyTimerHandle, TimerDelegate, 3.0f, false);

	// spanwer->notify
}

void AEntranceActor::InitializeAttributes()
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

	if (AttributeSet)
	{
		AttributeSet->SetHealth(AttributeSet->GetMaxHealth());

		UEnemyHPBarWidget* HPBarWidget = Cast<UEnemyHPBarWidget>(HPBar->GetUserWidgetObject());
		if (HPBarWidget)
		{
			HPBarWidget->SetMaxHealth(AttributeSet->GetMaxHealth(), true);
		}
	}

}

void AEntranceActor::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	float Health = Data.NewValue;

	// update HP Bar first
	UEnemyHPBarWidget* HPBarWidget = Cast<UEnemyHPBarWidget>(HPBar->GetUserWidgetObject());
	if (HPBarWidget)
	{
		HPBarWidget->SetHealth(Data.NewValue);
	}

	if (!IsAlive())
	{
		OnDied();
		return;
	}

	AudioComponent->Play();
}

float AEntranceActor::GetHealth() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetHealth();
	}
	return 0.0f;
}

bool AEntranceActor::IsAlive() const
{
	return GetHealth() > 0.0f;
}

UAbilitySystemComponent* AEntranceActor::GetAbilitySystemComponent() const
{
	return ASC;
}