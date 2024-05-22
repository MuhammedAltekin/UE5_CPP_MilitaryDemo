// Fill out your copyright notice in the Description page of Project Settings.


#include "SAM.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASAM::ASAM()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASAM::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASAM::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASAM::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASAM::CalculatePartRotation()
{
	if (TopObjectRef && BarrelObjectRef)
	{
		const float TurretTopRotYaw_B = UKismetMathLibrary::SelectFloat(UKismetMathLibrary::FindLookAtRotation(TopObjectRef->GetComponentLocation(), TargetLocation).Yaw, TurretTopFirstPosition, TargetIsVisible);
		TurretTopRotation = UKismetMathLibrary::RLerp(FRotator(0, TopObjectRef->GetComponentRotation().Yaw, 0), FRotator(0, TurretTopRotYaw_B, 0), 0.05f, true).Yaw;
		const float TurretBarrelRotPitch_B = UKismetMathLibrary::SelectFloat(UKismetMathLibrary::FindLookAtRotation(BarrelObjectRef->GetComponentLocation(), TargetLocation).Pitch, 0, TargetIsVisible);
		TurretBarrelRotation = UKismetMathLibrary::RLerp(FRotator(BarrelObjectRef->GetRelativeRotation().Pitch, 0, 0), FRotator(TurretBarrelRotPitch_B, 0, 0), .05f, true).Pitch;
	}
}

void ASAM::LookingForTarget()
{
	if (TopObjectRef)
	{
		const TArray<AActor*> IgnoreActor;
		FHitResult HitResult;
		const FVector EndLocation = TargetLocation + (UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::FindLookAtRotation(TopObjectRef->GetComponentLocation(), TargetLocation)) * 100);
		UKismetSystemLibrary::LineTraceSingle(GetWorld(), TopObjectRef->GetComponentLocation(), EndLocation,TraceTypeQuery1,false,IgnoreActor,EDrawDebugTrace::None,HitResult,true);
		if (HitResult.GetActor() == PlayerJet)
		{
			TargetIsVisible = true;
			if (bOnce_TurretRotatingStart && TurretSound)
			{
				bOnce_TurretRotatingStart = false;
				TurretSound->SetTriggerParameter(FName("TurretRotatingStart"));
				bOnce_TurretRotatingEnd = true;
			}
		}
		else
		{
			TargetIsVisible = false;
			if (bOnce_TurretRotatingEnd && TurretSound)
			{
				bOnce_TurretRotatingEnd = false;
				TurretSound->SetTriggerParameter(FName("TurretRotatingEnd"));
				bOnce_TurretRotatingStart = true;
			}
		}
	}
}

void ASAM::ScanningNewTarget()
{
	PlayerJet = Cast<AF15E>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (PlayerJet)
	{
		TargetLocation = PlayerJet->GetActorLocation();
		DistanceFromTarget = UKismetMathLibrary::Vector_Distance(GetActorLocation(), PlayerJet->GetActorLocation());
		if (DistanceFromTarget < RadarRange)
		{
			CreateTurretSound();
			LookingForTarget();
			TargetDetected = true;
		}
	}
	else
	{
		TargetDetected = false;
		TargetIsVisible = false;
	}
}

void ASAM::LockClear()
{
	TargetIsVisible = false;
	if (bOnce_TurretRotatingEnd && TurretSound)
	{
		bOnce_TurretRotatingEnd = false;
		TurretSound->SetTriggerParameter(FName("TurretRotatingEnd"));
		bOnce_TurretRotatingStart = true;
	}
}

void ASAM::TargetInSight()
{
	if (TopObjectRef && BarrelObjectRef)
	{
		TopObjectRef->SetRelativeRotation(FRotator(0, TurretTopRotation, 0));
		BarrelObjectRef->SetRelativeRotation(FRotator(TurretBarrelRotation, 0, 0));
	}
}


