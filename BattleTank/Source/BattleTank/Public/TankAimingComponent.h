// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

class UTankBarrelComponent;
class UTankTurretComponent;
class AProjectile;

UENUM()
enum class EFiringStatus : uint8
{
	Reloading, Aiming, Locked, OutOfAmmo
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialise(UTankBarrelComponent* BarrelToSet, UTankTurretComponent* TurretToSet);

	void AimAt(const FVector& HitLocation);

	UFUNCTION(BlueprintCallable)
	void Fire();

	EFiringStatus GetFiringState() const;

	UFUNCTION(BlueprintCallable)
	int GetAmmoLeft() const;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "State")
	EFiringStatus firingStatus = EFiringStatus::Reloading;

	UPROPERTY(BlueprintReadWrite, Category = "Setup")
	TSubclassOf<AProjectile> ProjectileBlueprint;

private:
	// Sets default values for this component's properties
	UTankAimingComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool IsBarrelMoving();
	void MoveBarrelTowards();

	UTankBarrelComponent* Barrel = nullptr;
	UTankTurretComponent* Turret = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float LaunchSpeed = 8000.f; // 80 m/s

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float ReloadTimeInSeconds = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float AmmoLeft = 3;

	double LastFireTime = 0;

	FVector AimDirection;
};
