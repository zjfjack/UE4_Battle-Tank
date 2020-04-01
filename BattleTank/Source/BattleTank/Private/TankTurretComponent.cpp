// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTurretComponent.h"

void UTankTurretComponent::Rotate(float RelativeSpeed)
{
	RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1, 1);
	auto AzimuthChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	auto NewAzimuth = RelativeRotation.Yaw + AzimuthChange;
	SetRelativeRotation(FRotator(0.f, NewAzimuth, 0.f));
}