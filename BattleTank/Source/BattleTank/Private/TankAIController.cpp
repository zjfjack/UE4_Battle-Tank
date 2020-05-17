// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"
#include "GameFramework/PlayerController.h"
#include "Tank.h"

void ATankAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
    auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();

    if (!ensure(AimingComponent && PlayerTank)) return;
        MoveToActor(PlayerTank, AcceptanceRadius);
        AimingComponent->AimAt(PlayerTank->GetActorLocation());

    if (AimingComponent->GetFiringState() == EFiringStatus::Locked)
        AimingComponent->Fire();
}

void ATankAIController::SetPawn(APawn* InPawn)
{
    Super::SetPawn(InPawn);
    if (InPawn)
    {
        auto PossessedTank = Cast<ATank>(InPawn);
        if (!PossessedTank) return;

        // Subscribe our local method to the tank's death event
        PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnPossessedTankDeath);
    }
}

void ATankAIController::OnPossessedTankDeath()
{
    auto pawn = GetPawn();
    if (!ensure(pawn))
        pawn->DetachFromControllerPendingDestroy();
}
