// Copyright (c) 2023 Team Light Seekers All rights reserved.

#include "Bomb.h"
#include "EnemyBase.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ABomb::ABomb()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	IsSet = false;
}

// Called when the game starts or when spawned
void ABomb::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsSet)
	{
		Timer -= DeltaTime;
		if (Timer < 0.0f)
		{
			Explode();
		}
	}
}

void ABomb::OnSet()
{
	IsSet = true;
	UKismetSystemLibrary::DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 12, FLinearColor::Green, Timer);
}

void ABomb::Explode()
{
	UKismetSystemLibrary::DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 12, FLinearColor::Red, 3.0f);

	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(Radius);

	TArray<FHitResult> HitResults;
	GetWorld()->SweepMultiByChannel(HitResults, GetActorLocation(), GetActorLocation(), FQuat(), ECollisionChannel::ECC_Pawn, CollisionShape);

	UE_LOG(LogTemp, Warning, TEXT("Bomb Explosion with Size: %i"), HitResults.Num());
	for (FHitResult& Hit : HitResults)
	{
		AEnemyBase* Target = Cast<AEnemyBase>(Hit.GetActor());
		if (Target)
		{
			Target->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*BombGameplayEffectSpecHandle.Data);
		}
	}

	FTransform Transform;
	Transform.SetLocation(GetActorLocation());

	AActor* VFXActor = GetWorld()->SpawnActor<AActor>(ExplosionVFXActor, Transform);

	Destroy();
	IsSet = false;
}
