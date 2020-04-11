// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"
#include "GameFramework/PlayerController.h"

void ATankAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
    auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();

    if (!ensure(AimingComponent && PlayerTank)) return;
        MoveToActor(PlayerTank, AcceptanceRadius);
        AimingComponent->AimAt(PlayerTank->GetActorLocation());

    UE_LOG(LogTemp, Warning, TEXT("FiringState %i"), AimingComponent->GetFiringState());

    if (AimingComponent->GetFiringState() == EFiringStatus::Locked)
        AimingComponent->Fire();
}