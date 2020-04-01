// Fill out your copyright notice in the Description page of Project Settings.


#include "Engine/World.h"
#include "Projectile.h"
#include "TankBarrelComponent.h"
#include "TankAimingComponent.h"
#include "Tank.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TankAimingComponent = CreateDefaultSubobject<UTankAimingComponent>(FName("Amiming Component"));
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATank::Fire()
{
	//UE_LOG(LogTemp, Warning, TEXT("Before Barrel check Tank name: %s"), *GetName());
	if (!Barrel) return;

	auto projectile = GetWorld()->SpawnActor<AProjectile>(
		ProjectileBlueprint, 
		Barrel->GetProjectileSocketLocation(), 
		Barrel->GetProjectileSocketRotation());
	//UE_LOG(LogTemp, Warning, TEXT("Tank name: %s"), *GetName());
	projectile->LaunchProjectile(LaunchSpeed);
}

void ATank::AimAt(const FVector& HitLocation)
{
	TankAimingComponent->AimAt(HitLocation, LaunchSpeed);
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATank::SetBarrelReference(UTankBarrelComponent* BarrelToSet)
{
	UE_LOG(LogTemp, Warning, TEXT("Set Barrel Tank name: %s"), *GetName());
	Barrel = BarrelToSet;
	TankAimingComponent->SetBarrelReference(BarrelToSet);
}

void ATank::SetTurretReference(UTankTurretComponent* TurretToSet)
{
	TankAimingComponent->SetTurretReference(TurretToSet);
}

