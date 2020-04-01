// Fill out your copyright notice in the Description page of Project Settings.


#include "TankBarrelComponent.h"

void UTankBarrelComponent::Elevate(float RelativeSpeed)
{
	RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1, 1);
	auto ElevationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	auto RawNewElevation = RelativeRotation.Pitch + ElevationChange;
	auto ClampedElevation = FMath::Clamp(RawNewElevation, MinElevationDegrees, MaxElevationDegrees);
	SetRelativeRotation(FRotator(ClampedElevation, 0.f, 0.f));
}

FVector UTankBarrelComponent::GetProjectileSocketLocation() const
{
	return this->GetSocketLocation(FName("Projectile"));
}

FRotator UTankBarrelComponent::GetProjectileSocketRotation() const
{
	return this->GetSocketRotation(FName("Projectile"));
}