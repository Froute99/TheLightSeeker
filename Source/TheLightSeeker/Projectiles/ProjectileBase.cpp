// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Characters/CharacterBase.h"
#include "Enemies/EnemyBase.h"
#include "CharacterAbilitySystemComponent.h"
#include "CharacterDefaultDamageEffect.h"

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
		UCharacterAbilitySystemComponent* EnemyASC = Cast<UCharacterAbilitySystemComponent>(Overlapped->GetAbilitySystemComponent());

		if (!EnemyASC)
		{
			UE_LOG(LogTemp, Log, TEXT("Enemy ASC was null"));
			return;
		}

		UCharacterAbilitySystemComponent* PlayerASC = Cast<UCharacterAbilitySystemComponent>(Cast<ACharacterBase>(GetInstigator())->GetAbilitySystemComponent());
		if (!PlayerASC)
		{
			UE_LOG(LogTemp, Log, TEXT("Player ASC was null"));
			return;
		}

		Overlapped->SetTargetPlayer(GetInstigator());
		//EnemyASC->ReceiveDamage()

	}

}

