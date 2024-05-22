// Fill out your copyright notice in the Description page of Project Settings.


#include "AAT.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "F15E.h"

#define Print(Key,Time,Color,Text) if(GEngine) GEngine->AddOnScreenDebugMessage(Key,Time,FColor::Color,TEXT(Text))
#define PrintF(Key,Time,Color,Type,Text) if(GEngine) GEngine->AddOnScreenDebugMessage(Key,Time,FColor::Color,FString::Printf(TEXT(Type),Text))
#define String(Text) FString(TEXT(Text))
#define OnceTimer(TimerHandle,Promph,Loop,Time,Delay) GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([=](){ Promph }), Time, Loop,Delay)


// Sets default values
AAAT::AAAT()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	Base->SetupAttachment(RootComponent);

	Top = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Top"));
	Top->SetupAttachment(Base);

	Barrel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Barrel"));
	Barrel->SetupAttachment(Top);

	BarrelTip = CreateDefaultSubobject<UArrowComponent>(TEXT("Barrel Tip"));
	BarrelTip->SetupAttachment(Barrel);

}
// Called when the game starts or when spawned
void AAAT::BeginPlay()
{
	Super::BeginPlay();

	// Captive Tick 60 Times Loop
	FTimerHandle CaptiveTickTH;
	OnceTimer(CaptiveTickTH,

		CaptiveTick();
	
	, true, 0.016667f, 0);

	// Fire Event Loop
	FTimerHandle FireTH;
	OnceTimer(FireTH,
	
		FireEvent();
	
	, true, FireRate, 0);

	// Begin Play

	TurretTopFirstRotation = Top->GetComponentRotation().Yaw;

	//Calling Jet
	
	PlayerJet = Cast<AF15E>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

// Called every frame
void AAAT::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAAT::CaptiveTick()
{
	if (PlayerJet)
	{
		CalculateEnemyInfo();
		CalculatePartRotation();
		TargetInSight();
	}
	
}

void AAAT::FireEvent()
{
	TargetInRange();
}

void AAAT::CalculateEnemyInfo()
{
	if (PlayerJet)
	{
		
		TargetLocation = PlayerJet->GetActorLocation();
		TargetRunPath = PlayerJet->GetActorLocation() + (PlayerJet->GetVelocity() * (DistanceFromTarget/FireRange));
		DistanceFromTarget = UKismetMathLibrary::Vector_Distance(GetActorLocation(), PlayerJet->GetActorLocation());

		if(DistanceFromTarget < RadarRange)
		{
			if (!DoOnce_1)
			{
				DoOnce_1 = true;
				TurretSound = UGameplayStatics::SpawnSoundAttached(TurretSoundSource, Barrel, FName("None"),FVector(0), FRotator(0), EAttachLocation::KeepRelativeOffset, false, 1, 1, 0, nullptr, nullptr, false);
			}
			TargetDetected = true;
			LookingForTarget();
		}
		else
		{
			TargetDetected = false;
			LockClear();
		}
	}
	else
	{
		TargetDetected = false;
		TargetIsVisible = false;
		FireRotationBool = false;
		LockClear();
	}
}

void AAAT::LookingForTarget()
{
	FVector TraceStart = Top->GetComponentLocation();
	FVector TraceEnd = TargetLocation + (100 * (UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::FindLookAtRotation(Top->GetComponentLocation(), TargetLocation))));
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	UKismetSystemLibrary::LineTraceSingle(GetWorld(),TraceStart,TraceEnd,ETraceTypeQuery::TraceTypeQuery1,false,ActorsToIgnore,EDrawDebugTrace::None,HitResult,true);
	if (PlayerJet == HitResult.GetActor())
	{	
		TargetIsVisible = true;
		if (!DoOnce_RotatingStart)
		{
			DoOnce_RotatingStart = true;
			TurretSound->SetTriggerParameter(FName("TurretRotatingStart"));
			DoOnce_RotatingEnd = false;
		}
		FTimerHandle TH;
		OnceTimer(TH, FireRotationBool = true;, false, 1, 1);
	}
	else
	{
		LockClear();
	}
}

void AAAT::LockClear()
{
	TargetIsVisible = false;

	if (!DoOnce_RotatingEnd)
	{
		DoOnce_RotatingEnd = true;
		TurretSound->SetTriggerParameter(FName("TurretRotatingEnd"));
		DoOnce_RotatingStart = false;
	}
	FireRotationBool = false;
}

void AAAT::CalculatePartRotation()
{
	float TopSelect = UKismetMathLibrary::SelectFloat(UKismetMathLibrary::FindLookAtRotation(Top->GetComponentLocation(), TargetRunPath).Yaw, TurretTopFirstRotation, TargetIsVisible);
	TurretTopRotation = UKismetMathLibrary::Lerp(Top->GetComponentRotation().Yaw,TopSelect,UKismetMathLibrary::SelectFloat(1,0.05f,FireRotationBool));
	float BarrelSelect = UKismetMathLibrary::SelectFloat(UKismetMathLibrary::FindLookAtRotation(Barrel->GetComponentLocation(),TargetRunPath).Pitch,0,TargetIsVisible);
	TurretBarrelRotation = UKismetMathLibrary::Lerp(Barrel->GetRelativeRotation().Pitch,BarrelSelect,UKismetMathLibrary::SelectFloat(1,0.05f,FireRotationBool));
}

void AAAT::TargetInSight()
{
	Barrel->SetRelativeRotation(FRotator(TurretBarrelRotation,0,0));
	Top->SetWorldRotation(FRotator(0, TurretTopRotation, 0));
}

void AAAT::TargetInRange()
{
	if (TargetIsVisible)
	{
		if (DistanceFromTarget < FireRange)
		{
			GetWorld()->SpawnActor<AActor>(Bullet,BarrelTip->GetComponentTransform());
			if (!DoOnce_FireStart)
			{
				DoOnce_FireStart = true;
				TurretSound->SetTriggerParameter(FName("FireStart"));
				DoOnce_FireEnd = false;
			}
		}
		else
		{
			ClearFire();
		}
	}
	else
	{
		ClearFire();
	}
}

void AAAT::ClearFire()
{
	if (!DoOnce_FireEnd)
	{
		DoOnce_FireEnd = true;
		TurretSound->SetTriggerParameter(FName("FireEnd"));
		DoOnce_FireStart = false;
	}
}

