// Fill out your copyright notice in the Description page of Project Settings.


#include "SAMRocket.h"

#include "Components/AudioComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASAMRocket::ASAMRocket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASAMRocket::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASAMRocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Target && MovementComp)
	{
		Distance = UKismetMathLibrary::Vector_Distance(GetActorLocation(), Target->GetActorLocation());
		TargetLocation = Distance / MovementComp->Velocity.Length() * Target->GetVelocity() + Target->GetActorLocation();
		FVector LerpB = 1.2 * UKismetMathLibrary::GetForwardVector(MovementComp->Velocity.Length() * UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation));
		MovementComp->Velocity = UKismetMathLibrary::VLerp(MovementComp->Velocity, LerpB,.05f);
	}
}

