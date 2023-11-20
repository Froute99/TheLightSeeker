// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"

#include "Actors/Characters/CharacterBase.h"
#include "Actors/Enemies/EnemyBase.h"

#include "GameAbilitySystem/CharacterAbilitySystemComponent.h"

#include "GameAbilitySystem/CharacterDefaultDamageEffect.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnBeginOverlap);
	RootComponent = BoxComponent;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	Mesh->SetupAttachment(RootComponent);


	if (!ProjectileMovementComponent)
	{
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
		ProjectileMovementComponent->InitialSpeed = 100.0f;
		ProjectileMovementComponent->MaxSpeed = 300.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->Bounciness = 0.3f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}

}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileBase::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AProjectileBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		AEnemyBase* Overlapped = Cast<AEnemyBase>(OtherActor);

		if (!IsValid(Overlapped)) return;

		if (!DamageEffectSpecHandle.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("%s() DamageEffectSpecHandle not created: %s."), *FString(__FUNCTION__), *GetName());
			return;
		}


		//if (Overlapped->StaticClass() == AEnemyBase::StaticClass())		// redundant check
		{
			UE_LOG(LogTemp, Log, TEXT("Overlapped with enemy"));
			UCharacterAbilitySystemComponent* EnemyASC = Cast<UCharacterAbilitySystemComponent>(Overlapped->GetAbilitySystemComponent());

			if (!EnemyASC)
			{
				UE_LOG(LogTemp, Log, TEXT("ASC was null"));
				return;
			}

			UCharacterAbilitySystemComponent* PlayerASC = Cast<UCharacterAbilitySystemComponent>(Cast<ACharacterBase>(GetInstigator())->GetAbilitySystemComponent());
			PlayerASC->ApplyGameplayEffectSpecToTarget(*DamageEffectSpecHandle.Data.Get(), EnemyASC);


			//EnemyASC->ReceiveDamage()

			//if (!IsValid(ProjectileEffect))
			//{
			//	UE_LOG(LogTemp, Error, TEXT("%s() Missing Projectile Effect for %s."), *FString(__FUNCTION__), *GetName());
			//	return;
			//}
			


			
			//ACharacterBase* Player = Cast<ACharacterBase>(GetInstigator());
			//Player->GetAbilitySystemComponent()->ApplyGameplayEffectToTarget(ProjectileEffect, Overlapped->GetAbilitySystemComponent());
		}
	}

}

