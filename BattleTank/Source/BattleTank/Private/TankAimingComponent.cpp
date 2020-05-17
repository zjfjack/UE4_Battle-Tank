// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile.h"
#include "TankBarrelComponent.h"
#include "TankTurretComponent.h"
#include "TankAimingComponent.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();
	LastFireTime = FPlatformTime::Seconds();
}

void UTankAimingComponent::Initialise(UTankBarrelComponent* BarrelToSet, UTankTurretComponent* TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (AmmoLeft == 0)
	{
		firingStatus = EFiringStatus::OutOfAmmo;
		return;
	}
	bool bIsReloading = (FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds;
	if (bIsReloading)
		firingStatus = EFiringStatus::Reloading;
	else if (IsBarrelMoving())
		firingStatus = EFiringStatus::Aiming;
	else
		firingStatus = EFiringStatus::Locked;
}

EFiringStatus UTankAimingComponent::GetFiringState() const
{
	return firingStatus;
}

int32 UTankAimingComponent::GetAmmoLeft() const
{
	return AmmoLeft;
}

bool UTankAimingComponent::IsBarrelMoving()
{
	if (!ensure(Barrel)) return false;
	
	auto BarrelForward = Barrel->GetForwardVector();
	return !BarrelForward.Equals(AimDirection, 0.1);
}


void UTankAimingComponent::AimAt(const FVector& HitLocation)
{
	if (!ensure(Barrel)) return;

	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetProjectileSocketLocation();
	bool bHasAimSolution = UGameplayStatics::SuggestProjectileVelocity(
		this, OutLaunchVelocity, StartLocation, HitLocation, LaunchSpeed, false, 0, 0, ESuggestProjVelocityTraceOption::DoNotTrace);
	if (bHasAimSolution)
	{
		AimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrelTowards();
	}
}

void UTankAimingComponent::Fire()
{
	if (!ensure(Barrel && ProjectileBlueprint)) return;
	if (firingStatus == EFiringStatus::Reloading || firingStatus == EFiringStatus::OutOfAmmo) return;

	LastFireTime = FPlatformTime::Seconds();
	auto projectile = GetWorld()->SpawnActor<AProjectile>(
		ProjectileBlueprint,
		Barrel->GetProjectileSocketLocation(),
		Barrel->GetProjectileSocketRotation());
	projectile->LaunchProjectile(LaunchSpeed);
	AmmoLeft -= 1;
}

void UTankAimingComponent::MoveBarrelTowards()
{
	if (!ensure(Barrel && Turret)) return;

	auto BarrelRotation = Barrel->GetForwardVector().Rotation();
	auto AimRotation = AimDirection.Rotation();
	auto DeltaRotation = AimRotation - BarrelRotation;

	Barrel->Elevate(DeltaRotation.Pitch);

	float DeltaRoationYaw = DeltaRotation.Yaw;
	if (abs(DeltaRoationYaw) > 180) {
		DeltaRoationYaw += DeltaRoationYaw > 0 ? -360 : 360;
	}
	
	Turret->Rotate(DeltaRoationYaw);
}
