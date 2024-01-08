// Fill out your copyright notice in the Description page of Project Settings.

#include "F15E.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


#include "Components/AudioComponent.h"
#include "Components/DecalComponent.h"

#define Print(Key,Time,Color,Text) if(GEngine) GEngine->AddOnScreenDebugMessage(Key,Time,FColor::Color,TEXT(Text))
#define PrintF(Key,Time,Color,Type,Text) if(GEngine) GEngine->AddOnScreenDebugMessage(Key,Time,FColor::Color,FString::Printf(TEXT(Type),Text))
#define String(Text) FString(TEXT(Text))
#define OnceTimer(TimerHandle,Promph,Loop,Time,Delay) GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([=](){ Promph }), Time, Loop,Delay)

// Sets default values
AF15E::AF15E()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Root"));
	Root->SetupAttachment(RootComponent);
	
	PlaneCollision = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneCollision"));
	PlaneCollision->SetupAttachment(Root);

	SensorCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SensorCamera"));
	SensorCamera->SetupAttachment(Root);

	GroundCheck = CreateDefaultSubobject<UCapsuleComponent>(TEXT("GroundCheck"));
	GroundCheck->SetupAttachment(Root);

	FrontWheel = CreateDefaultSubobject<USphereComponent>(TEXT("FrontWheel"));
	FrontWheel->SetupAttachment(Root);

	F15E_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("F15E Mesh"));
	F15E_Mesh->SetupAttachment(Root);

	RightWheel = CreateDefaultSubobject<USphereComponent>(TEXT("RightWheel"));
	RightWheel->SetupAttachment(FrontWheel);

	LeftWheel = CreateDefaultSubobject<USphereComponent>(TEXT("LeftWheel"));
	LeftWheel->SetupAttachment(FrontWheel);

	AimDot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AimDot"));
	AimDot->SetupAttachment(F15E_Mesh);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(F15E_Mesh);

	PilotCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PilotCamera"));
	PilotCamera->SetupAttachment(CameraBoom);

	MiddlePanel = CreateDefaultSubobject<UDecalComponent>(TEXT("MiddlePanel"));
	MiddlePanel->SetupAttachment(F15E_Mesh);

	RightPanel = CreateDefaultSubobject<UDecalComponent>(TEXT("RightPanel"));
	RightPanel->SetupAttachment(MiddlePanel);

	LeftPanel = CreateDefaultSubobject<UDecalComponent>(TEXT("LeftPanel"));
	LeftPanel->SetupAttachment(MiddlePanel);

	Missiles = CreateDefaultSubobject<UArrowComponent>(FName("Missiles"));
	Missiles->SetupAttachment(F15E_Mesh);

	AGM_1 = CreateDefaultSubobject<USkeletalMeshComponent>(FName("AGM_1"));
	AGM_1->SetupAttachment(Missiles);

	AGM_2 = CreateDefaultSubobject<USkeletalMeshComponent>(FName("AGM_2"));
	AGM_2->SetupAttachment(Missiles);

	AGM_3 = CreateDefaultSubobject<USkeletalMeshComponent>(FName("AGM_3"));
	AGM_3->SetupAttachment(Missiles);

	AGM_4 = CreateDefaultSubobject<USkeletalMeshComponent>(FName("AGM_4"));
	AGM_4->SetupAttachment(Missiles);

	APM_1 = CreateDefaultSubobject<USkeletalMeshComponent>(FName("APM_1"));
	APM_1->SetupAttachment(Missiles);

	APM_2 = CreateDefaultSubobject<USkeletalMeshComponent>(FName("APM_2"));
	APM_2->SetupAttachment(Missiles);

	APM_3 = CreateDefaultSubobject<USkeletalMeshComponent>(FName("APM_3"));
	APM_3->SetupAttachment(Missiles);

	APM_4 = CreateDefaultSubobject<USkeletalMeshComponent>(FName("APM_4"));
	APM_4->SetupAttachment(Missiles);

	APM_5 = CreateDefaultSubobject<USkeletalMeshComponent>(FName("APM_5"));
	APM_5->SetupAttachment(Missiles);

	APM_6 = CreateDefaultSubobject<USkeletalMeshComponent>(FName("APM_6"));
	APM_6->SetupAttachment(Missiles);

	APM_7 = CreateDefaultSubobject<USkeletalMeshComponent>(FName("APM_7"));
	APM_7->SetupAttachment(Missiles);

	APM_8 = CreateDefaultSubobject<USkeletalMeshComponent>(FName("APM_8"));
	APM_8->SetupAttachment(Missiles);

	GAGM_1 = CreateDefaultSubobject<USkeletalMeshComponent>(FName("GAGM_1"));
	GAGM_1->SetupAttachment(Missiles);

	GAGM_2 = CreateDefaultSubobject<USkeletalMeshComponent>(FName("GAGM_2"));
	GAGM_2->SetupAttachment(Missiles);

	BOMB = CreateDefaultSubobject<UStaticMeshComponent>(FName("BOMB"));
	BOMB->SetupAttachment(Missiles);
}

// Called when the game starts or when spawned
void AF15E::BeginPlay()
{
	Super::BeginPlay();

	//----------------Setting Input------------------------//

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	//---------------60 Times Per Sec Tick--------------------------//
	
	OnceTimer(CaptiveTickTimer, CaptiveTick(); , true, 0.0166f, 0);
	//---------------Add Jet Sound------------------------------//

	GroundCheckOverlap();
}


// Called every frame
void AF15E::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AF15E::CPP_Possessed()
{
	if (this == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		//----------------Setting Input------------------------//

		if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
		//---------------60 Times Per Sec Tick--------------------------//
		OnceTimer(CaptiveTickTimer, CaptiveTick();, true, 0.0166f, 0);
		//---------------Add Jet Sound------------------------------//
		UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MasterClassMix, MasterClass, .1f, 1, 0);
		UGameplayStatics::PushSoundMixModifier(GetWorld(), MasterClassMix);
		UGameplayStatics::SetSoundMixClassOverride(GetWorld(), CockpitClassMix, CockpitClass, 1, 1, 0);
		UGameplayStatics::PushSoundMixModifier(GetWorld(), CockpitClassMix);

		GroundCheckOverlap();

	}
}

void AF15E::ClearAllTimer()
{
	GetWorldTimerManager().ClearTimer(EngineOffDelayTh);
	GetWorldTimerManager().ClearTimer(EngineOnDelayTh);
	GetWorldTimerManager().ClearTimer(MachineGunTh);
	GetWorldTimerManager().ClearTimer(ZoomTH);
	GetWorldTimerManager().ClearTimer(ViewTH);
	GetWorldTimerManager().ClearTimer(WheelTH);
	GetWorldTimerManager().ClearTimer(TakeOffTH);
	GetWorldTimerManager().ClearTimer(LockCamTH);
	GetWorldTimerManager().ClearTimer(MissileTH);
	GetWorldTimerManager().ClearTimer(LockedTH);
	GetWorldTimerManager().ClearTimer(CaptiveTickTimer);
	Destroyed = true;
}

// Called to bind functionality to input
void AF15E::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{

		
		EnhancedInputComponent->BindAction(IA_Increase_Power, ETriggerEvent::Triggered, this, &AF15E::IncreasePowerTriggered);
		EnhancedInputComponent->BindAction(IA_Increase_Power, ETriggerEvent::Started, this, &AF15E::IncreasePowerStarted);
		EnhancedInputComponent->BindAction(IA_Increase_Power, ETriggerEvent::Completed, this, &AF15E::IncreasePowerCompleted);

		EnhancedInputComponent->BindAction(IA_Decrease_Power, ETriggerEvent::Triggered, this, &AF15E::DecreasePowerTriggered);
		EnhancedInputComponent->BindAction(IA_Decrease_Power, ETriggerEvent::Started, this, &AF15E::DecreasePowerStarted);
		EnhancedInputComponent->BindAction(IA_Decrease_Power, ETriggerEvent::Completed, this, &AF15E::DecreasePowerCompleted);


		EnhancedInputComponent->BindAction(IA_Engine_On, ETriggerEvent::Started, this, &AF15E::EngineOnEvent);
		EnhancedInputComponent->BindAction(IA_Engine_Off, ETriggerEvent::Started, this, &AF15E::EngineOffEvent);

		EnhancedInputComponent->BindAction(IA_Up_Down, ETriggerEvent::Triggered, this, &AF15E::UpDownTriggered);
		EnhancedInputComponent->BindAction(IA_Left_Right, ETriggerEvent::Triggered, this, &AF15E::LeftRightTriggered);
		EnhancedInputComponent->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &AF15E::TurnTriggered);
		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AF15E::LookTriggered);

		EnhancedInputComponent->BindAction(IA_FreeCam, ETriggerEvent::Started, this, &AF15E::FreeCamEvent);

		EnhancedInputComponent->BindAction(IA_Gun, ETriggerEvent::Triggered, this, &AF15E::GunTriggered);

		EnhancedInputComponent->BindAction(IA_Zoom, ETriggerEvent::Started, this, &AF15E::ZoomStarted);
		EnhancedInputComponent->BindAction(IA_Zoom, ETriggerEvent::Completed, this, &AF15E::ZoomCompleted);

		EnhancedInputComponent->BindAction(IA_ViewChange, ETriggerEvent::Started, this, &AF15E::ViewChangeStarted);

		//EnhancedInputComponent->BindAction(IA_Wheel, ETriggerEvent::Started, this, &AF15E::WheelStarted);

		EnhancedInputComponent->BindAction(IA_SensorCam, ETriggerEvent::Started, this, &AF15E::SensorCamStarted);

		EnhancedInputComponent->BindAction(IA_LockSensorCam, ETriggerEvent::Started, this, &AF15E::LockSensorCamStarted);

		EnhancedInputComponent->BindAction(IA_SensorCamZoomIn, ETriggerEvent::Triggered, this, &AF15E::SensorCamZoomInStarted);
		EnhancedInputComponent->BindAction(IA_SensorCamZoomOut, ETriggerEvent::Triggered, this, &AF15E::SensorCamZoomOutStarted);

		EnhancedInputComponent->BindAction(IA_ChangeWeaponOrder, ETriggerEvent::Started, this, &AF15E::ChangeWeaponOrderEvent);

		EnhancedInputComponent->BindAction(IA_MouseControl, ETriggerEvent::Triggered, this, &AF15E::MouseControlTriggered);
	}
}

void AF15E::CaptiveTick()
{
	if (!Destroyed && !Paused)
	{
		PhysicForces();
		EngineForces();
		ThrusterParticle();
		SettingEnginePower();
		AngleCompensation();
		SettingControl();

		Speed = Velocity.Length();

		if (JetSound)
		{
			JetSound->SetFloatParameter(FName("SpeedLevel"), Speed / 12500);
		}
	}
}

void AF15E::IncreasePowerTriggered()
{
	if (Power)
	{
		if (JetSound)
		{
			JetSound->SetFloatParameter(TEXT("SoundMultply"), UKismetMathLibrary::FClamp(AccelerationMultiply/100 , 0.1f , 1));
		}
	}
}

void AF15E::IncreasePowerStarted()
{
	PowerUp = true;
}

void AF15E::IncreasePowerCompleted()
{
	PowerUp = false;
}

void AF15E::DecreasePowerTriggered()
{
	if (JetSound)
	{
		JetSound->SetFloatParameter(TEXT("SoundMultply"), UKismetMathLibrary::FClamp(AccelerationMultiply / 100, 0.1f, 1));
	}
}

void AF15E::DecreasePowerStarted()
{
	PowerDown = true;
}

void AF15E::DecreasePowerCompleted()
{
	
	PowerDown = false;
}

void AF15E::EngineOnEvent()
{
	if (!EngineStart)
	{
		EngineStart = true;
		JetSound->SetTriggerParameter(TEXT("EngineOn"));
		LeftExhaust->SetActive(true,true);
		RightExhaust->SetActive(true,true);
		AccelerationMultiply = 1;

		if (FlipFlop_2 == 0)
		{
			JetSound->SetTriggerParameter(TEXT("CockpitOn"));
		}

		OnceTimer(EngineOnDelayTh, 
		Power = true;
		GetWorld()->GetTimerManager().ClearTimer(EngineOnDelayTh);
		,false, 1, 6);
	}
}

void AF15E::EngineOffEvent()
{
	if (EngineStart)
	{
		EngineStart = false;
		JetSound->SetTriggerParameter(TEXT("EngineOff"));
		JetSound->SetTriggerParameter(TEXT("EngineOff"));
		JetSound->SetTriggerParameter(TEXT("AfterburnerOff"));
		JetSound->SetTriggerParameter(TEXT("CockpitOff"));
		Afterburner = false;
		LeftExhaust->SetActive(false, false);
		RightExhaust->SetActive(false, false);

		OnceTimer(EngineOffDelayTh,
			AccelerationMultiply = 0;
			EngineSound = 0.1f;
			Power = false;
			GetWorld()->GetTimerManager().ClearTimer(EngineOffDelayTh);
		, false, 1, 6);
	}
}

void AF15E::UpDownTriggered(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		FVector Vector = MovementVector.X * UpDownControl * UpDownControlMultply * Root->GetRightVector();
		Vector = UKismetMathLibrary::VLerp(FVector(0), Vector, 0.1f);
		Root->AddTorqueInDegrees(Vector, TEXT(""), true);
	}
}

void AF15E::LeftRightTriggered(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		FVector Vector = MovementVector.X * LeftRightControl * Root->GetUpVector() / 2;
		Vector = UKismetMathLibrary::VLerp(FVector(0), Vector, 0.1f);
		Root->AddTorqueInDegrees(Vector, TEXT(""), true);
	}
}

void AF15E::TurnTriggered(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		FVector Vector = MovementVector.X * TurnControl * TurnControlMultply * Root->GetForwardVector();
		Vector = UKismetMathLibrary::VLerp(FVector(0), Vector, 0.1f);
		Root->AddTorqueInDegrees(Vector, TEXT(""), true);
	}
}

void AF15E::LookTriggered(const FInputActionValue& Value)
{
	bLooking = true;
	FVector2d Axis = UKismetMathLibrary::SelectFloat(SensivityMultiplyer,1,SensorCamOpen) * Value.Get<FVector2D>();
	if (FreeLook)
	{
		CalculateRotation();

		USceneComponent* Object;
		if (SensorCamOpen)
		{
			Object = SensorCamera;

		}
		else
		{
			Object = CameraBoom;

		}
		FRotator RelativeRot = Object->GetRelativeRotation();

		float PitchValue = UKismetMathLibrary::SelectFloat(Axis.Y + RelativeRot.Pitch, PitchRot, LockPitch);
		float YawValue = UKismetMathLibrary::SelectFloat(Axis.X + RelativeRot.Yaw, YawRot, LockYaw);

		Object->SetRelativeRotation(FRotator(PitchValue, YawValue, 0));
	}
	else if (bSensorCamLock)
	{
		if (SensorCamOpen)
		{
			USceneComponent* Object = SensorCamera;

			FRotator RelativeRot = Object->GetRelativeRotation();

			float PitchValue = UKismetMathLibrary::SelectFloat(Axis.Y + RelativeRot.Pitch, PitchRot, LockPitch);
			float YawValue = UKismetMathLibrary::SelectFloat(Axis.X + RelativeRot.Yaw, YawRot, LockYaw);

			Object->SetRelativeRotation(FRotator(PitchValue, YawValue, 0));

			TArray<AActor*> ActorsToIgnore;
			UKismetSystemLibrary::LineTraceSingle(GetWorld(), SensorCamera->GetComponentLocation(), SensorCamera->GetComponentLocation() + SensorCamera->GetForwardVector() * 1000000000, ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::None, LockCamHR, true);
			if (LockCamHR.GetComponent())
			{
				if (LockCamHR.GetComponent()->GetCollisionObjectType() == ECollisionChannel::ECC_Vehicle)
				{
					CamTargetLocation = LockCamHR.GetActor()->GetActorLocation();
					TargetName = LockCamHR.GetComponent()->GetName();
					bLockedOnTarget = true;
					if (!Once_1)
					{
						UGameplayStatics::PlaySound2D(GetWorld(), D_LockedTargetSound);
						Once_1 = true;
						OnceTimer(LockedTH,
							UGameplayStatics::PlaySound2D(GetWorld(), D_LockedTargetSound_2);
						Once_1 = false;
						GetWorldTimerManager().ClearTimer(LockedTH);
						, false, 1, 3);
					}

				}
				else
				{
					CamTargetLocation = LockCamHR.Location;
					TargetName = FString("None");
					bLockedOnTarget = false;
				}
			}

			
		}
	}
}

void AF15E::MouseControlTriggered(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>() / 2;
	if (SensorCamOpen || Speed < 50)
	{
	}
	else
	{
		FVector Vector_1 = UKismetMathLibrary::VLerp(FVector(0), Root->GetRightVector() * ((UpDownControl / 4) * MovementVector.Y), .1f);
		Root->AddTorqueInDegrees(Vector_1 * -1, FName(""), true);

		FVector OnceVector = MovementVector.X * ((Root->GetUpVector() * (LeftRightControl / 5))); // --- ((TurnControl / 5) * (Root->GetForwardVector() * -1)) +
		FVector Vector_2 = UKismetMathLibrary::VLerp(FVector(0), UKismetMathLibrary::SelectVector((OnceVector / 2), OnceVector, IsGrounded), .1f);
		Root->AddTorqueInDegrees(Vector_2, FName(""), true);
	}
}

void AF15E::GroundCheckOverlap()
{
	GroundCheck->OnComponentBeginOverlap.AddDynamic(this, &AF15E::GroundCheckBeginEvent);
	GroundCheck->OnComponentEndOverlap.AddDynamic(this, &AF15E::GroundCheckEndEvent);
}

void AF15E::GroundCheckBeginEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IsGrounded = true;
	bTakeOffCheck = false;
	GetWorldTimerManager().ClearTimer(TakeOffTH);
}

void AF15E::GroundCheckEndEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IsGrounded = false;
	OnceTimer(TakeOffTH,
		if (!IsGrounded)
		{
			bTakeOffCheck = true;
		}
	, false, 1, 3);
}

void AF15E::CalculateRotation()
{
	USceneComponent* Object;
	if (SensorCamOpen)
	{
		Object = SensorCamera;
		FRotator RelativeRot = Object->GetRelativeRotation();

		//---------------------Checking Pitch-------------------//

		if (RelativeRot.Pitch < 10)
		{
			LockPitch = true;
			if (RelativeRot.Pitch > -89)
			{
				LockPitch = true;
			}
			else
			{
				LockPitch = false;
				PitchRot = -88;
			}
		}
		else
		{
			LockPitch = false;
			PitchRot = 9;
		}

		//---------------------Checking Yaw--------------------//

		/*if (RelativeRot.Yaw > -130)
		{
			if (RelativeRot.Yaw < 130)
			{
				LockYaw = true;
			}
			else
			{
				LockYaw = false;
				YawRot = 129;
			}
		}
		else
		{
			LockYaw = false;
			YawRot = -129;
		}*/
	}
	else
	{
		Object = CameraBoom;
		FRotator RelativeRot = Object->GetRelativeRotation();

		//---------------------Checking Pitch-------------------//

		if (RelativeRot.Pitch < 75)
		{
			if (RelativeRot.Pitch > -35)
			{
				LockPitch = true;
			}
			else
			{
				LockPitch = false;
				PitchRot = -34;
			}
		}
		else
		{
			LockPitch = false;
			PitchRot = 74;
		}

		//---------------------Checking Yaw--------------------//

		if (RelativeRot.Yaw > -130)
		{
			if (RelativeRot.Yaw < 130)
			{
				LockYaw = true;
			}
			else
			{
				LockYaw = false;
				YawRot = 129;
			}
		}
		else
		{
			LockYaw = false;
			YawRot = -129;
		}
	}

	
}

void AF15E::FreeCamEvent()
{
	if (!SensorCamOpen)
	{
		if (FlipFlop_1 == 0)
		{
			FlipFlop_1 = 1;

			FreeLook = true;
		}
		else if (FlipFlop_1 == 1)
		{
			FlipFlop_1 = 0;

			FreeLook = false;
			CameraBoom->SetRelativeRotation(FRotator(-5, 0, 0));
		}
	}
}

void AF15E::GunTriggered()
{

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	FVector StartLine = AimDot->GetComponentLocation();
	FVector EndLine = StartLine + (10000000 * UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::FindLookAtRotation(F15E_Mesh->GetSocketLocation(FName("CamLoc")), AimDot->GetComponentLocation())));
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartLine, EndLine, ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::None, MachineGunHR, true);

	if (WeaponOrder == 0)
	{
		MachineGunEvent();
	}
	else
	{
		if (!bMissileDelay)
		{
			if (WeaponOrder == 1)
			{
				AGM_Event();
			}
			else if (WeaponOrder == 2)
			{
				GAGM_Event();
				UGameplayStatics::SpawnSoundAttached(ReleaseBombSound, F15E_Mesh, FName(""), FVector(0), FRotator(0), EAttachLocation::KeepRelativeOffset, true, 1);
				if (bLockedOnTarget)
				{
					UGameplayStatics::PlaySound2D(GetWorld(), D_ShootTargetSound);
				}
				
			}
			else if (WeaponOrder == 3)
			{
				BOMB_Event();
				UGameplayStatics::SpawnSoundAttached(ReleaseBombSound, F15E_Mesh, FName(""), FVector(0), FRotator(0), EAttachLocation::KeepRelativeOffset, true, 1);
			}
			else if (WeaponOrder == 4)
			{
				APM_Event();
			}
			bMissileDelay = true;
			OnceTimer(MissileTH,
				bMissileDelay = false;
			,false,1,MissileDelayTime);
		}
		
	}
	
}

void AF15E::ZoomStarted()
{
	GetWorldTimerManager().ClearTimer(ZoomTH);
	OnceTimer(ZoomTH,
			CurrentFov = UKismetMathLibrary::FInterpTo(CurrentFov,75, GetWorld()->GetDeltaSeconds(), 20);
			PilotCamera->SetFieldOfView(CurrentFov);
	, true, .0166f, 0);
	
}

void AF15E::ZoomCompleted()
{
		GetWorldTimerManager().ClearTimer(ZoomTH);
		OnceTimer(ZoomTH,
			CurrentFov = UKismetMathLibrary::FInterpTo(CurrentFov, 135, GetWorld()->GetDeltaSeconds(), 20);
		PilotCamera->SetFieldOfView(CurrentFov);
		, true, .0166f, 0);

}

void AF15E::ViewChangeStarted()
{
	if (!SensorCamOpen)
	{
		if (FlipFlop_2 == 0)
		{
			FlipFlop_2 = 1;
			
			GetWorldTimerManager().ClearTimer(ViewTH);
			if (EngineStart)
			{
				JetSound->SetTriggerParameter(FName("CockpitOff"));
			}
			UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MasterClassMix, MasterClass, 1, 1, 0);
			UGameplayStatics::PushSoundMixModifier(GetWorld(), MasterClassMix);
			UGameplayStatics::SetSoundMixClassOverride(GetWorld(), CockpitClassMix, CockpitClass, 0, 1, 0);
			UGameplayStatics::PushSoundMixModifier(GetWorld(), CockpitClassMix);
			
			OnceTimer(ViewTH,
				CameraBoom->TargetArmLength = UKismetMathLibrary::FInterpTo(CameraBoom->TargetArmLength,2500,GetWorld()->GetDeltaSeconds(),20);
		
			,true,.0166f,0);
		}
		else if (FlipFlop_2 == 1)
		{
			FlipFlop_2 = 0;

			if (EngineStart)
			{
				JetSound->SetTriggerParameter(FName("CockpitOn"));
			}
			UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MasterClassMix, MasterClass, .1f, 1, 0);
			UGameplayStatics::PushSoundMixModifier(GetWorld(), MasterClassMix);
			UGameplayStatics::SetSoundMixClassOverride(GetWorld(), CockpitClassMix, CockpitClass, 1, 1, 0);
			UGameplayStatics::PushSoundMixModifier(GetWorld(), CockpitClassMix);
			GetWorldTimerManager().ClearTimer(ViewTH);
			OnceTimer(ViewTH,
				CameraBoom->TargetArmLength = UKismetMathLibrary::FInterpTo(CameraBoom->TargetArmLength, 10, GetWorld()->GetDeltaSeconds(), 20);

			, true, .0166f, 0);
		}
	}
	
}

void AF15E::WheelStarted()
{
	if (!WheelDoOnce)
	{
		WheelDoOnce = true;


		if(!FlipFlop_3)
		{
			FlipFlop_3 = true;

			Wheel = false;
			LeftWheel->SetCollisionEnabled(ECollisionEnabled::ProbeOnly);
			RightWheel->SetCollisionEnabled(ECollisionEnabled::ProbeOnly);
			FrontWheel->SetCollisionEnabled(ECollisionEnabled::ProbeOnly);
		}
		else if (FlipFlop_3)
		{
			FlipFlop_3 = false;

			Wheel = true;
			LeftWheel->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			RightWheel->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			FrontWheel->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
		

		OnceTimer(WheelTH,
			WheelDoOnce = false;
			GetWorldTimerManager().ClearTimer(WheelTH);
		,false,1,5);
	}
}	  

void AF15E::SensorCamStarted()
{
	if (!FlipFlop_4)
	{
		FlipFlop_4 = true;

		PilotCamera->SetActive(false);
		SensorCamera->SetActive(true);
		SensorCamOpen = true;
		//FreeLook = true;
	}
	else if (FlipFlop_4)
	{
		FlipFlop_4 = false;

		PilotCamera->SetActive(true);
		SensorCamera->SetActive(false);
		SensorCamOpen = false;
	}
}

void AF15E::PhysicForces()
{
	
	if (!Destroyed && !Paused)
	{
		//--------------------------------Gravity-------------------------------------------------//
		float GravityZ = UKismetMathLibrary::FClamp((((Speed / 10000) - 1) * -1) * GravityScale, -4000, 0);
		FVector Gravity = FVector(0, 0, GravityZ);
		Root->AddImpulse(Gravity);

		//------------------------------Drag--------------------------------------//
		FVector DragVector = FVector((Velocity.Length() / 135000 * UKismetMathLibrary::SelectFloat(DragMultiply * 2, DragMultiply, IsGrounded)) * (Velocity * -1));
		Root->AddImpulse(DragVector);

		//-----------------------------Smooth Angle---------------------------//
		Root->AddTorqueInDegrees(FVector(Root->GetPhysicsAngularVelocityInDegrees() * -1 / 0.5f), FName(""), true);

		//----------------------------Set Velocity Variable-----------------//
		Velocity = Root->GetPhysicsLinearVelocity();

	}
	
}

void AF15E::EngineForces()
{
	if (Power)
	{
		Root->AddImpulse(Root->GetForwardVector() * AccelerationMultiply * SpeedMultiply);
	}
}

void AF15E::SettingEnginePower()
{
	if (Power)
	{
		if (PowerUp)
		{
			AccelerationMultiply = UKismetMathLibrary::FClamp(AccelerationMultiply + .5f, MinPower,MaxPower);
			if (Speed >= 36000)
			{
				SpeedMultiply = 100;
				if (!Afterburner)
				{
					Afterburner = true;
					JetSound->SetTriggerParameter(FName("AfterburnerOn"));
				}
				
			}
			else
			{
				SpeedMultiply = 65;
				if (Afterburner)
				{
					Afterburner = false;
					JetSound->SetTriggerParameter(FName("AfterburnerOff"));
				}
			}
			
		}
		else
		{
			if (PowerDown)
			{
				AccelerationMultiply = UKismetMathLibrary::FClamp(AccelerationMultiply - .5f, MinPower, MaxPower);
				if (Speed < 25000){
					SpeedMultiply = 50;
					if (Afterburner)
					{
						Afterburner = false;
						JetSound->SetTriggerParameter(FName("AfterburnerOff"));
					}
				}
			}
		}
	}
}

void AF15E::ThrusterParticle()
{
	if (RightExhaust && LeftExhaust)
	{
		FVector JetScaleVector = FVector(ThrusterScale * (AccelerationMultiply / 100));
		RightExhaust->SetNiagaraVariableVec3(String("JetScale"), JetScaleVector);
		RightExhaust->SetNiagaraVariableFloat(String("RingOpacity"), AccelerationMultiply / 100 * .05f);
		RightExhaust->SetNiagaraVariableFloat(String("SpriteScale"), AccelerationMultiply / 100);
		LeftExhaust->SetNiagaraVariableVec3(String("JetScale"), JetScaleVector);
		LeftExhaust->SetNiagaraVariableFloat(String("RingOpacity"), AccelerationMultiply / 100 * .05f);
		LeftExhaust->SetNiagaraVariableFloat(String("SpriteScale"), AccelerationMultiply / 100);
		CurrentColor = UKismetMathLibrary::VLerp(CurrentColor, UKismetMathLibrary::SelectVector(FVector(50, 0, 0), FVector(10, 0, 0), Afterburner), 0.1f);
		RightExhaust->SetNiagaraVariableVec3(String("Color"), CurrentColor);
		LeftExhaust->SetNiagaraVariableVec3(String("Color"), CurrentColor);

		if (Speed < 25000 && Afterburner)
		{
			SpeedMultiply = 50;
			Afterburner = false;
			JetSound->SetTriggerParameter(FName("AfterburnerOff"));
		}
	}
	
}

void AF15E::AngleCompensation()
{
	if (!bTakeOffCheck || Speed <= 13500)
	{
		FVector LocalVelocity = FVector((Speed - 5) * Root->GetForwardVector());
		Root->SetPhysicsLinearVelocity(UKismetMathLibrary::VLerp(Velocity, FVector(LocalVelocity.X, LocalVelocity.Y, Velocity.Z), Speed / 500000));
	}
	
	else
	{
		FVector LocalVelocity = FVector((Speed - 5) * Root->GetForwardVector());
		Root->SetPhysicsLinearVelocity(UKismetMathLibrary::VLerp(Velocity, LocalVelocity, Speed / 500000));
	}
	
	
}

void AF15E::SettingControl()
{
	//-----------------UP-DOWN-CONTROL-------------//
	if (Speed <= 8000)
	{
		UpDownControl = Speed / 8000 * 800;
	}
	else
	{
		UpDownControl = ((Speed / -90000) + 1) * 800;
	}

	//----------------Left-Right-Control--------------//
	if (IsGrounded)
	{
		LeftRightControl = ((Speed / -15000) + 1) * 1000;
	}
	else
	{
		LeftRightControl = ((Speed / -150000) + 1) * 300;
	}

	//--------------Turn Control--------------------//
	if (IsGrounded)
	{
		TurnControl = 0;
	}
	else
	{
		TurnControl = UKismetMathLibrary::FClamp((Speed / 30000) * 3000,1200,3000);
	}

}

void AF15E::LockSensorCamStarted()
{
	if (SensorCamOpen)
	{
		if (!FlipFlop_5)
		{
			FlipFlop_5 = true;

			//Locking Setting

			bSensorCamLock = true;
			FreeLook = false;
			TArray<AActor*> ActorsToIgnore;
			UKismetSystemLibrary::LineTraceSingle(GetWorld(), SensorCamera->GetComponentLocation(), SensorCamera->GetComponentLocation() + SensorCamera->GetForwardVector() * 1000000000, ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::None, LockCamHR, true);
			if (LockCamHR.GetComponent())
			{
				if (LockCamHR.GetComponent()->GetCollisionObjectType() == ECollisionChannel::ECC_Vehicle)
				{
					CamTargetLocation = LockCamHR.GetActor()->GetActorLocation();
					TargetName = LockCamHR.GetComponent()->GetName();
					bLockedOnTarget = true;
					UGameplayStatics::PlaySound2D(GetWorld(), D_LockedTargetSound);
					OnceTimer(LockedTH,
						UGameplayStatics::PlaySound2D(GetWorld(), D_LockedTargetSound_2);
					GetWorldTimerManager().ClearTimer(LockedTH);
					, false, 1, 3);
				}
				else
				{
					CamTargetLocation = LockCamHR.Location;
					TargetName = FString("None");
					bLockedOnTarget = false;
				}

				OnceTimer(LockCamTH,

					FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(SensorCamera->GetComponentLocation(), CamTargetLocation);
				SensorCamera->SetWorldRotation(Rotation);

				CalculateRotation();

				USceneComponent * Object;
				Object = SensorCamera;
				FRotator RelativeRot = Object->GetRelativeRotation();

				float PitchValue = UKismetMathLibrary::SelectFloat(RelativeRot.Pitch, PitchRot, LockPitch);
				float YawValue = UKismetMathLibrary::SelectFloat(RelativeRot.Yaw, YawRot, LockYaw);

				Object->SetRelativeRotation(FRotator(PitchValue, YawValue, 0));

				, true, 0.003f, 0);
			}
			

		}
		else
		{

			//Clear Locked

			FlipFlop_5 = false;
			bSensorCamLock = false;
			bLockedOnTarget = false;
			FreeLook = true;
			GetWorldTimerManager().ClearTimer(LockCamTH);
		}
	}
}

void AF15E::SensorCamZoomInStarted()
{
	
	SensivityMultiplyer = UKismetMathLibrary::FClamp(SensivityMultiplyer - .002f, .02f, 1);
	SensorCamera->SetFieldOfView(UKismetMathLibrary::FClamp(SensorCamera->FieldOfView - .2f, 2, 90));
	
}

void AF15E::SensorCamZoomOutStarted()
{
	SensivityMultiplyer = UKismetMathLibrary::FClamp(SensivityMultiplyer + .002f, .02f, 1);
	SensorCamera->SetFieldOfView(UKismetMathLibrary::FClamp(SensorCamera->FieldOfView + .2f, 2, 90));
	
}

void AF15E::MachineGunEvent()
{
	if (FireAgain)
	{
		FireAgain = false;

		FVector SpawnLoc = F15E_Mesh->GetSocketLocation(FName("MachineGun"));
		FRotator SpawnRot = UKismetMathLibrary::FindLookAtRotation(F15E_Mesh->GetSocketLocation(FName("MachineGun")), MachineGunHR.Location);
		GetWorld()->SpawnActor<AActor>(Bullet, SpawnLoc, SpawnRot);
		UGameplayStatics::SpawnSoundAttached(MachineGunSound, F15E_Mesh, FName("GunSound"), FVector(0), FRotator(0), EAttachLocation::KeepRelativeOffset, true, 1);
		UGameplayStatics::SpawnEmitterAttached(MuzzleParticle, F15E_Mesh, FName("Muzzle"),FVector(0),FRotator(0),FVector(1),EAttachLocation::SnapToTarget);
		OnceTimer(MachineGunTh,
			FireAgain = true;
		GetWorld()->GetTimerManager().ClearTimer(MachineGunTh);
		, false, 1, MachineGunDensity);
	}
}

void AF15E::AGM_Event()
{
	USkeletalMeshComponent* Object = nullptr;
	switch (AGM_Remain)
	{
	case 4:
		Object = AGM_1;
		break;

	case 3:
		Object = AGM_2;
		break;

	case 2:
		Object = AGM_3;
		break;

	case 1:
		Object = AGM_4;
		break;

	default:
		break;
	}
	if (AGM_Remain != 0)
	{
		AGM_Remain -= 1;
		FRotator SpawnRot = UKismetMathLibrary::FindLookAtRotation(Object->GetComponentLocation(), MachineGunHR.Location);
		GetWorld()->SpawnActor<AActor>(A_AGM,Object->GetComponentLocation(),SpawnRot);
		Object->DestroyComponent();
	}
}

void AF15E::APM_Event()
{
	USkeletalMeshComponent* Object = nullptr;
	switch (APM_Remain)
	{
	case 8:
		Object = APM_1;
		break;

	case 7:
		Object = APM_2;
		break;

	case 6:
		Object = APM_3;
		break;

	case 5:
		Object = APM_4;
		break;

	case 4:
		Object = APM_5;
		break;

	case 3:
		Object = APM_6;
		break;

	case 2:
		Object = APM_7;
		break;

	case 1:
		Object = APM_8;
		break;

	default:
		break;
	}
	if (APM_Remain != 0)
	{
		APM_Remain -= 1;
		FRotator SpawnRot = UKismetMathLibrary::FindLookAtRotation(Object->GetComponentLocation(), MachineGunHR.Location);
		GetWorld()->SpawnActor<AActor>(A_APM, Object->GetComponentLocation(), SpawnRot);
		Object->DestroyComponent();
	}
}

void AF15E::GAGM_Event()
{
	USkeletalMeshComponent* Object = nullptr;
	switch (GAGM_Remain)
	{
	
	case 2:
		Object = GAGM_1;
		break;

	case 1:
		Object = GAGM_2;
		break;

	default:
		break;
	}
	if (GAGM_Remain != 0)
	{
		GAGM_Remain -= 1;
		GetWorld()->SpawnActor<AActor>(A_GAGM, Object->GetComponentLocation(), Object->GetComponentRotation());
		Object->DestroyComponent();
	}
}

void AF15E::BOMB_Event()
{
	UStaticMeshComponent* Object = nullptr;
	Object = BOMB;
		
	if (BOMB_Remain != 0)
	{
		BOMB_Remain -= 1;
		GetWorld()->SpawnActor<AActor>(A_BOMB, Object->GetComponentLocation(), Object->GetComponentRotation());
		Object->DestroyComponent();
	}
}

void AF15E::ChangeWeaponOrderEvent()
{
	if (WeaponOrder >= 0 && WeaponOrder < 4)
	{
		
		WeaponOrder += 1;
	}
	else
	{
		WeaponOrder = 0;
	}

	switch (WeaponOrder)
	{

	case 0:

		WeaponName = FString("30mm Cannon");
		MissileDelayTime = .5f;
		break;

	case 1:

		WeaponName = FString("AGM");
		break;

	case 2:

		WeaponName = FString("GAGM");
		break;

	case 3:

		WeaponName = FString("BOMB");
		break;

	case 4:

		WeaponName = FString("APM");
		MissileDelayTime = .25f;
		break;

	default:
		break;
	}
}














