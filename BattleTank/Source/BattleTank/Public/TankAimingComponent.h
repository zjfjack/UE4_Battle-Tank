// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

class UTankBarrelComponent;
class UTankTurretComponent;

UENUM()
enum class EFiringStatus : uint8
{
	Reloading, Aiming, Locked
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialise(UTankBarrelComponent* BarrelToSet, UTankTurretComponent* TurretToSet);

	void AimAt(const FVector& HitLocation);
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "State")
	EFiringStatus firingStatus = EFiringStatus::Aiming;

private:
	// Sets default values for this component's properties
	UTankAimingComponent();

	UTankBarrelComponent* Barrel = nullptr;
	UTankTurretComponent* Turret = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float LaunchSpeed = 8000.f; // 80 m/s

	void MoveBarrelTowards(const FVector& AimDirection);
};
