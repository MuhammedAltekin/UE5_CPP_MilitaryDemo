         // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "NiagaraComponent.h"
#include "F15E.generated.h"

UCLASS()
class VEHICLETUT_API AF15E : public APawn
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		// ReSharper disable once UnrealHeaderToolError
		class UInputMappingContext* DefaultMappingContext;

public:
	// Sets default values for this pawn's properties
	AF15E();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;

public:	



	//-------------------Input---------------------------//

	UPROPERTY(EditDefaultsOnly)
		class UInputAction* IA_Increase_Power;

	UPROPERTY(EditDefaultsOnly)
		class UInputAction* IA_Decrease_Power;

	UPROPERTY(EditDefaultsOnly)
		class UInputAction* IA_Engine_On;

	UPROPERTY(EditDefaultsOnly)
		class UInputAction* IA_Engine_Off;

	UPROPERTY(EditDefaultsOnly)
		class UInputAction* IA_Turn;

	UPROPERTY(EditDefaultsOnly)
		class UInputAction* IA_Left_Right;

	UPROPERTY(EditDefaultsOnly)
		class UInputAction* IA_Up_Down;

	UPROPERTY(EditDefaultsOnly)
		class UInputAction* IA_MouseControl;

	UPROPERTY(EditDefaultsOnly)
		class UInputAction* IA_Look;

	UPROPERTY(EditDefaultsOnly)
		class UInputAction* IA_FreeCam;

	UPROPERTY(EditDefaultsOnly)
		class UInputAction* IA_Gun;

	UPROPERTY(EditDefaultsOnly)
		class UInputAction* IA_Zoom;

	UPROPERTY(EditDefaultsOnly)
		class UInputAction* IA_ViewChange;

	UPROPERTY(EditDefaultsOnly)
		class UInputAction* IA_Wheel;

	UPROPERTY(EditDefaultsOnly)
		class UInputAction* IA_SensorCam;

	UPROPERTY(EditDefaultsOnly)
		class UInputAction* IA_LockSensorCam;

	UPROPERTY(EditDefaultsOnly)
		class UInputAction* IA_SensorCamZoomIn;

	UPROPERTY(EditDefaultsOnly)
		class UInputAction* IA_SensorCamZoomOut;

	UPROPERTY(EditDefaultsOnly)
		class UInputAction* IA_ChangeWeaponOrder;;
	//-----------------------FUNCTIONS----------------------//

	UFUNCTION()
		void CaptiveTick();

	UFUNCTION()
		void IncreasePowerTriggered();

	UFUNCTION()
		void IncreasePowerStarted();

	UFUNCTION()
		void IncreasePowerCompleted();

	UFUNCTION()
		void DecreasePowerTriggered();

	UFUNCTION()
		void DecreasePowerStarted();

	UFUNCTION()
		void DecreasePowerCompleted();

	UFUNCTION()
		void EngineOnEvent();

	UFUNCTION()
		void EngineOffEvent();

	UFUNCTION()
		void UpDownTriggered(const FInputActionValue& Value);

	UFUNCTION()
		void LeftRightTriggered(const FInputActionValue& Value);

	UFUNCTION()
		void TurnTriggered(const FInputActionValue& Value);

	UFUNCTION()
		void LookTriggered(const FInputActionValue& Value);

	UFUNCTION()
		void MouseControlTriggered(const FInputActionValue& Value);

	UFUNCTION()
		void GroundCheckOverlap();
	

	UFUNCTION()
		void GroundCheckBeginEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void GroundCheckEndEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void CalculateRotation();

	UFUNCTION()
		void FreeCamEvent();
	
	UFUNCTION()
		void GunTriggered();

	UFUNCTION()
		void ZoomStarted();

	UFUNCTION()
		void ZoomCompleted();

	UFUNCTION()
		void ViewChangeStarted();

	UFUNCTION()
		void WheelStarted();

	UFUNCTION()
		void SensorCamStarted();

	UFUNCTION()
		void PhysicForces();

	UFUNCTION()
		void EngineForces();

	UFUNCTION()
		void SettingEnginePower();

	UFUNCTION()
		void ThrusterParticle();

	UFUNCTION()
		void AngleCompensation();

	UFUNCTION()
		void SettingControl();

	UFUNCTION()
		void LockSensorCamStarted();

	UFUNCTION()
		void SensorCamZoomInStarted();

	UFUNCTION()
		void SensorCamZoomOutStarted();

	UFUNCTION()
		void MachineGunEvent();

	UFUNCTION()
		void AGM_Event();

	UFUNCTION()
		void APM_Event();

	UFUNCTION()
		void GAGM_Event();

	UFUNCTION()
		void BOMB_Event();

	UFUNCTION()
		void ChangeWeaponOrderEvent();

	UFUNCTION(BlueprintCallable)
		void CPP_Possessed();

	UFUNCTION(BlueprintCallable)
		void ClearAllTimer();
	
	//----------------------Components-----------------------//

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
		UAudioComponent* JetSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UCapsuleComponent* Root;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* PlaneCollision;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* AimDot;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
		class UCameraComponent* SensorCamera;

	UPROPERTY(EditDefaultsOnly)
		UCapsuleComponent* GroundCheck;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
		USphereComponent* FrontWheel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		USphereComponent* RightWheel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		USphereComponent* LeftWheel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		USkeletalMeshComponent* F15E_Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UNiagaraComponent* LeftExhaust;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UNiagaraComponent* RightExhaust;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
		class USpringArmComponent* CameraBoom;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UCameraComponent* PilotCamera;

	UPROPERTY(EditDefaultsOnly)
		UDecalComponent* MiddlePanel;

	UPROPERTY(EditDefaultsOnly)
		UDecalComponent* LeftPanel;

	UPROPERTY(EditDefaultsOnly)
		UDecalComponent* RightPanel;

	UPROPERTY(EditDefaultsOnly)
		UArrowComponent* Missiles;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		USkeletalMeshComponent* AGM_1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		USkeletalMeshComponent* AGM_2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		USkeletalMeshComponent* AGM_3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		USkeletalMeshComponent* AGM_4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		USkeletalMeshComponent* APM_1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		USkeletalMeshComponent* APM_2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		USkeletalMeshComponent* APM_3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		USkeletalMeshComponent* APM_4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		USkeletalMeshComponent* APM_5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		USkeletalMeshComponent* APM_6;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		USkeletalMeshComponent* APM_7;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		USkeletalMeshComponent* APM_8;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UStaticMeshComponent* BOMB;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		USkeletalMeshComponent* GAGM_1;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
		USkeletalMeshComponent* GAGM_2;

	
	//-------------------Variables--------------------//

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool Power;

	UPROPERTY(VisibleAnywhere)
		bool PowerUp;

	UPROPERTY(VisibleAnywhere)
		bool PowerDown;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
		float AccelerationMultiply;

	UPROPERTY()
		float EngineSound;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
		bool EngineStart;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
		bool Afterburner;

	UPROPERTY()
		FTimerHandle EngineOffDelayTh;

	UPROPERTY()
		FTimerHandle EngineOnDelayTh;

	UPROPERTY()
		float UpDownControl;

	UPROPERTY()
		float TurnControl;

	UPROPERTY(EditDefaultsOnly)
		float TurnControlMultply;

	UPROPERTY(EditDefaultsOnly)
		float UpDownControlMultply;

	UPROPERTY()
		float LeftRightControl;

	UPROPERTY(VisibleAnywhere)
		bool IsGrounded = true;

	UPROPERTY(VisibleAnywhere)
		bool FreeLook;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool SensorCamOpen;

	UPROPERTY()
		bool LockPitch;

	UPROPERTY()
		bool LockYaw;

	UPROPERTY()
		float PitchRot;

	UPROPERTY()
		float YawRot;

	UPROPERTY()
		int FlipFlop_1;

	UPROPERTY()
		FTimerHandle MachineGunTh;

	UPROPERTY()
		bool FireAgain = true;

	UPROPERTY()
		FHitResult MachineGunHR;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AActor> Bullet;

	UPROPERTY(EditDefaultsOnly)
		USoundBase* MachineGunSound;

	UPROPERTY(EditDefaultsOnly)
		float MachineGunDensity = 0.075f;

	UPROPERTY()
		float CurrentFov = 135;

	UPROPERTY()
		bool bZoom;

	UPROPERTY()
		FTimerHandle ZoomTH;

	UPROPERTY()
		int FlipFlop_2 = 0;

	UPROPERTY()
		FTimerHandle ViewTH;

	UPROPERTY()
		bool WheelDoOnce;

	UPROPERTY()
		bool FlipFlop_3;

	UPROPERTY()
		FTimerHandle WheelTH;

	UPROPERTY(BlueprintReadWrite)
		bool Wheel = true;

	UPROPERTY()
		bool FlipFlop_4;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
		float Speed;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
		float GravityScale = -1500;

	UPROPERTY(VisibleAnywhere)
		FVector Velocity;

	UPROPERTY(VisibleAnywhere)
		float SpeedMultiply = 50;

	UPROPERTY()
		float ThrusterScale = 0.2f;

	UPROPERTY()
		FVector CurrentColor = FVector(10,0,0);

	UPROPERTY()
		float MinPower = 1;

	UPROPERTY()
		float MaxPower = 100;

	UPROPERTY()
		bool bTakeOffCheck;

	UPROPERTY()
		FTimerHandle TakeOffTH;

	UPROPERTY(EditDefaultsOnly)
		float DragMultiply = 1;

	UPROPERTY(BlueprintReadOnly)
		bool bSensorCamLock;

	UPROPERTY()
		bool FlipFlop_5;

	UPROPERTY()
		FTimerHandle LockCamTH;

	UPROPERTY()
		FHitResult LockCamHR;

	UPROPERTY()
		bool bLooking;

	UPROPERTY(BlueprintReadOnly)
		FVector CamTargetLocation;

	UPROPERTY(VisibleAnywhere)
		float SensivityMultiplyer = 1;

	UPROPERTY(BlueprintReadOnly)
		bool bLockedOnTarget;

	UPROPERTY(BlueprintReadOnly)
		int	WeaponOrder;

	UPROPERTY(BlueprintReadOnly)
		FString WeaponName = FString("30mm Cannon");
	
	UPROPERTY(BlueprintReadOnly)
		FString TargetName = FString("None");

	UPROPERTY()
		bool bAGM;

	UPROPERTY(BlueprintReadOnly)
		int AGM_Remain = 4;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AActor> A_AGM;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AActor> A_GAGM;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AActor> A_APM;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AActor> A_BOMB;

	UPROPERTY()
		bool bAPM;

	UPROPERTY(BlueprintReadOnly)
		int APM_Remain = 8;

	UPROPERTY()
		bool bGAGM;

	UPROPERTY(BlueprintReadOnly)
		int GAGM_Remain = 2;

	UPROPERTY()
		bool bBOMB;

	UPROPERTY(BlueprintReadOnly)
		int BOMB_Remain = 1;

	UPROPERTY()
		FTimerHandle MissileTH;

	UPROPERTY()
		bool bMissileDelay;

	UPROPERTY(EditDefaultsOnly)
		USoundBase* AGMSound;

	UPROPERTY(EditDefaultsOnly)
		USoundBase* APMSound;

	UPROPERTY(EditDefaultsOnly)
		USoundBase* GAGMSound;

	UPROPERTY(EditDefaultsOnly)
		USoundBase* BOMBSound;

	UPROPERTY(EditDefaultsOnly)
		USoundBase* ReleaseBombSound;

	UPROPERTY()
		FTimerHandle LockedTH;

	UPROPERTY(EditDefaultsOnly)
		USoundBase* D_LockedTargetSound;

	UPROPERTY(EditDefaultsOnly)
		USoundBase* D_LockedTargetSound_2;

	UPROPERTY(EditDefaultsOnly)
		USoundBase* D_ShootTargetSound;

	UPROPERTY()
		bool Once_1;

	UPROPERTY(EditDefaultsOnly)
		float MissileDelayTime = .5f;

	UPROPERTY(EditDefaultsOnly)
		USoundClass* MasterClass;

	UPROPERTY(EditDefaultsOnly)
		USoundMix* MasterClassMix;

	UPROPERTY(EditDefaultsOnly)
		USoundClass* CockpitClass;

	UPROPERTY(EditDefaultsOnly)
		USoundMix* CockpitClassMix;

	UPROPERTY()
		bool Destroyed;

	UPROPERTY(EditDefaultsOnly)
		UParticleSystem* MuzzleParticle;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
		bool PlayAsJet;

	UPROPERTY(BlueprintReadWrite)
		bool Paused;

	UPROPERTY()
		FTimerHandle CaptiveTickTimer;
};
