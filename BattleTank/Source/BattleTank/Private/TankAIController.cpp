// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "Engine/World.h"
#include "Tank.h"
#include "GameFramework/PlayerController.h"

void ATankAIController::BeginPlay()
{
    Super::BeginPlay();
    auto PlayerTank = GetPlayerTank();

    if (PlayerTank)
    {
        UE_LOG(LogTemp, Warning, TEXT("AIController found player: %s"), *PlayerTank->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AIController can't find player tank"));
    }   
}

void ATankAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    auto PlayerTank = GetPlayerTank();
    if (PlayerTank)
    {
        auto ControlledTank = GetControlledTank();
        ControlledTank->AimAt(PlayerTank->GetActorLocation());
        ControlledTank->Fire();
    }
       
}


ATank* ATankAIController::GetControlledTank() const
{
    return Cast<ATank>(GetPawn());
}

ATank* ATankAIController::GetPlayerTank() const
{
    auto PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
    if (!PlayerPawn) 
        return nullptr;
    return Cast<ATank>(PlayerPawn);
}

