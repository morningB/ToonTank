// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

UCLASS()
class TOONTANKS_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();
	
	UPROPERTY(VisibleAnywhere,Category = "super duper variables", BlueprintReadWrite)
	int32 VisibleAnywhereInt = 12;

	UPROPERTY(EditAnywhere,Category = "super duper variables", BlueprintReadOnly)
	int32 EditAnywhereInt = 22;

	UPROPERTY(VisibleInstanceOnly)
	int32 VisibleInstanceOnlyInt = 32;

	UPROPERTY(VisibleDefaultsOnly)
	int32 VisibleDefaultOnlyInt = 42;

	UPROPERTY(EditDefaultsOnly)
	int32 EditDefaultOnlyInt = 52;
	
	UPROPERTY(EditInstanceOnly)
	int32 EditInstanceOnlyInt = 62;
	
	UPROPERTY(BlueprintReadWrite)
	float asf = 123.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



private:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;
};
