// Fill out your copyright notice in the Description page of Project Settings.


#include "Engine/World.h"
#include "Projectile.h"
#include "TankBarrelComponent.h"
#include "Tank.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ATank::Fire()
{
	if (!ensure(Barrel)) return;
	bool isReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds;
	if (!isReloaded) return;

	LastFireTime = FPlatformTime::Seconds();
	auto projectile = GetWorld()->SpawnActor<AProjectile>(
		ProjectileBlueprint, 
		Barrel->GetProjectileSocketLocation(), 
		Barrel->GetProjectileSocketRotation());
	projectile->LaunchProjectile(LaunchSpeed);
}