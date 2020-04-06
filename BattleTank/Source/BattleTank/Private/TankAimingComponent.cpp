// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TankBarrelComponent.h"
#include "TankTurretComponent.h"
#include "TankAimingComponent.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UTankAimingComponent::Initialise(UTankBarrelComponent* BarrelToSet, UTankTurretComponent* TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

void UTankAimingComponent::AimAt(const FVector& HitLocation, float LaunchSpeed)
{
	if (!ensure(Barrel))
		return;

	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetProjectileSocketLocation();
	bool bHasAimSolution = UGameplayStatics::SuggestProjectileVelocity(
		this, OutLaunchVelocity, StartLocation, HitLocation, LaunchSpeed, false, 0, 0, ESuggestProjVelocityTraceOption::DoNotTrace);
	if (bHasAimSolution)
	{
		auto AimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(AimDirection);
	}
}

void UTankAimingComponent::MoveBarrelTowards(const FVector& AimDirection)
{
	if (!ensure(Barrel && Turret))
		return;

	auto BarrelRotation = Barrel->GetForwardVector().Rotation();
	auto AimRotation = AimDirection.Rotation();
	auto DeltaRotation = AimRotation - BarrelRotation;

	Barrel->Elevate(DeltaRotation.Pitch);
	Turret->Rotate(DeltaRotation.Yaw);
}
