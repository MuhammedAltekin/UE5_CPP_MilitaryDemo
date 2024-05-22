// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "F15E.h"
#include "GameFramework/Pawn.h"
#include "SAM.generated.h"

UCLASS()
class VEHICLETUT_API ASAM : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASAM();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintImplementableEvent)
	void CreateTurretSound();
	UFUNCTION(BlueprintCallable)
	void CalculatePartRotation();
	UFUNCTION(BlueprintCallable)
	void LookingForTarget();
	UFUNCTION(BlueprintCallable)
	void TargetInSight();
	UFUNCTION(BlueprintCallable)
	void ScanningNewTarget();

	void LockClear();


	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool TargetDetected;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector TargetLocation;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TurretTopRotation;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TurretBarrelRotation;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float DistanceFromTarget;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AF15E* PlayerJet;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool TargetIsVisible;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float RadarRange;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float FireRange;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector Run_Path;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TurretTopFirstPosition;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool RocketLaunched;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int RocketSocketIndex;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAudioComponent* TurretSound;
	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	UStaticMeshComponent* TopObjectRef;
	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	UStaticMeshComponent* BarrelObjectRef;

	bool bOnce_TurretRotatingStart = true;
	bool bOnce_TurretRotatingEnd = true;
};
