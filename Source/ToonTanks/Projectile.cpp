// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
// Sets default values
AProjectile::AProjectile()
{
 	
	PrimaryActorTick.bCanEverTick = false;
	
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	RootComponent = ProjectileMesh;

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	
	ProjectileComponent->MaxSpeed = 1300.f;
	ProjectileComponent->InitialSpeed = 1300.f; // 최대 속도 넘으면 안됨.

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Display, TEXT("%s"),*HitComp->GetName());
	UE_LOG(LogTemp, Display, TEXT("%s"),*OtherActor->GetName());
	UE_LOG(LogTemp, Display, TEXT("%s"),*OtherComp->GetName());
}