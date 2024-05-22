// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "F15E.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SAMRocket.generated.h"

UCLASS()
class VEHICLETUT_API ASAMRocket : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASAMRocket();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	UAudioComponent* LoopSound;
	UPROPERTY(BlueprintReadWrite)
	UProjectileMovementComponent* MovementComp;

	AActor* Target;
	FVector TargetLocation;
	float Distance;
	AF15E* Jet = nullptr;
};
