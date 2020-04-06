// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "TankAimingComponent.h"
#include "Engine/World.h"
#include "Tank.h"

void ATankPlayerController::BeginPlay()
{
    Super::BeginPlay();
    auto AimingComponent = GetControlledTank()->FindComponentByClass<UTankAimingComponent>();
    if (ensure(AimingComponent))
        FoundAimingComponent(AimingComponent);
}

void ATankPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    AimTowardsCrosshair();
}


ATank* ATankPlayerController::GetControlledTank() const
{
    return Cast<ATank>(GetPawn());
}

// De-project the screen position of the crosshair to a world direction
bool ATankPlayerController::GetLookDirection(const FVector2D& ScreenLocation, FVector& OutLookDirection) const
{
    FVector CameraWorldLocation;
    return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, CameraWorldLocation, OutLookDirection);
}

bool ATankPlayerController::GetLookVectorHitLocation(const FVector& LookDirection, FVector& OutHitLocation) const
{
    FHitResult HitResult;
    FVector StartLocation = PlayerCameraManager->GetCameraLocation();
    // Vector add
    FVector EndLocation = StartLocation + LookDirection * LineTraceRange;

    if (GetWorld()->LineTraceSingleByChannel(
        HitResult,
        StartLocation,
        EndLocation,
        ECC_Visibility))
    {
        OutHitLocation = HitResult.Location;
        return true;
    }
    return false;
}

// Get world location of linetrace through crosshair
bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const
{
    // Find the crosshair position in pixel coordinates
    int32 ViewportSizeX, ViewportSizeY;
    GetViewportSize(ViewportSizeX, ViewportSizeY);
    auto ScreenLocation = FVector2D(ViewportSizeX * CrosshairXLocation, ViewportSizeY * CrosshairYLocation);
    
    FVector LookDirection;
    if (GetLookDirection(ScreenLocation, LookDirection))
    {
        // Line-trace along that look direction, and see what we hit (up to max range)
        return GetLookVectorHitLocation(LookDirection, OutHitLocation);
    }
    return false;
}

void ATankPlayerController::AimTowardsCrosshair()
{
    auto controlledTank = GetControlledTank();
    if (!ensure(controlledTank)) return;

    FVector HitLocation;

    // If it hits the landscape, tell controlled tank to aim at this point
    if (GetSightRayHitLocation(HitLocation))
        controlledTank->AimAt(HitLocation);
}