// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "F15E.h"
#include "NiagaraComponent.h"
#include "Components/ArrowComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AAT.generated.h"

UCLASS()
class VEHICLETUT_API AAAT : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAAT();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// OBJECTS

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* Base;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* Top;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* Barrel;

	UPROPERTY(EditDefaultsOnly)
		UArrowComponent* BarrelTip;

	// FUNCTÝONS

	UFUNCTION()
		void CaptiveTick();

	UFUNCTION()
		void FireEvent();

	UFUNCTION()
		void CalculateEnemyInfo();

	UFUNCTION()
		void LookingForTarget();

	UFUNCTION()
		void LockClear();

	UFUNCTION()
		void CalculatePartRotation();

	UFUNCTION()
		void TargetInSight();

	UFUNCTION()
		void TargetInRange();

	UFUNCTION()
		void ClearFire();
	// VARÝABLES

	UPROPERTY()
		float TurretTopFirstRotation;

	UPROPERTY(BlueprintReadOnly)
		AF15E* PlayerJet;
	
	UPROPERTY(BlueprintReadOnly)
		FVector TargetLocation;

	UPROPERTY(BlueprintReadOnly)
		FVector TargetRunPath;

	UPROPERTY(VisibleAnywhere)
		float DistanceFromTarget = 10000000;

	UPROPERTY(VisibleAnywhere)
		bool TargetDetected;

	UPROPERTY(VisibleAnywhere)
		bool TargetIsVisible = true;

	UPROPERTY()
		bool FireRotationBool;

	UPROPERTY(EditDefaultsOnly)
		float RadarRange = 300000;

	UPROPERTY(EditDefaultsOnly)
		float FireRange = 150000;

	UPROPERTY()
		float TurretTopRotation;

	UPROPERTY()
		float TurretBarrelRotation;

	UPROPERTY(EditDefaultsOnly)
		float FireRate = 0.066667f;

	UPROPERTY(EditDefaultsOnly)
		USoundBase* TurretSoundSource;

	UPROPERTY()
		UAudioComponent* TurretSound;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AActor> Bullet;

	//MACRO VARÝABLES

	UPROPERTY()
		bool DoOnce_1;

	UPROPERTY()
		bool DoOnce_RotatingStart;

	UPROPERTY()
		bool DoOnce_RotatingEnd = true;

	UPROPERTY()
		bool DoOnce_FireStart;

	UPROPERTY()
		bool DoOnce_FireEnd = true;
};
