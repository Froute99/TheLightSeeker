// Copyright (c) 2023 Team Light Seekers All rights reserved.

#include "Items/LightningTransferProjectile.h"
#include "Components/BoxComponent.h"
#include "Enemies/EnemyBase.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ALightningTransferProjectile::ALightningTransferProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent->OnComponentBeginOverlap.Clear(); // remove original overlap function
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ALightningTransferProjectile::OnBeginOverlap_Override);

	ProjectileMovementComponent->bIsHomingProjectile = true;
}

// Called when the game starts or when spawned
void ALightningTransferProjectile::BeginPlay()
{
	Super::BeginPlay();

	NextTargetIndex = 0;
	if (TargetEnemies.Num() == 0)
	{
		Destroy();
	}
}

// Called every frame
void ALightningTransferProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALightningTransferProjectile::OnBeginOverlap_Override(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	AEnemyBase* EnemyBase = Cast<AEnemyBase>(OtherActor);
	if (!EnemyBase)
	{
		return;
	}

	TargetEnemies[0]->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*DamageEffectSpecHandle.Data.Get(), EnemyBase->GetAbilitySystemComponent());
	
	++NextTargetIndex;

	if (NextTargetIndex < TargetEnemies.Num())
	{
		FVector ToNextTarget = TargetEnemies[NextTargetIndex]->GetActorLocation() - GetActorLocation();
		//FireInDirection(ToNextTarget.GetSafeNormal());
		ProjectileMovementComponent->HomingTargetComponent = TargetEnemies[NextTargetIndex]->GetRootComponent();
	}
	else
	{
		SetVelocity(FVector::ZeroVector);
		BoxComponent->OnComponentBeginOverlap.Clear(); // prevent later collision
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]() { if(this) this->Destroy(); }), 0.5f, true);
	}
}
