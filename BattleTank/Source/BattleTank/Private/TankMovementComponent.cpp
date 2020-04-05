// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTrackComponent.h"
#include "TankMovementComponent.h"

void UTankMovementComponent::Initialise(UTankTrackComponent* LeftTrackToSet, UTankTrackComponent* RightTrackToSet)
{
	LeftTrack = LeftTrackToSet;
	RightTrack = RightTrackToSet;
}

void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
	auto TankForward = GetOwner()->GetActorForwardVector().GetSafeNormal();
	auto AIForwardIntention = MoveVelocity.GetSafeNormal();

	/* 
	a•b = |a| * |b| * cosθ, |a| and |b| are 1 because of GetSafeNormal, so the result is cosθ
	cosθ is positive when θ = 0-90, 270-360 which means should move forward
	cosθ is negative when θ = 90-180, which means should move backward
	the absolute value of cosθ is only for the strength of moving forward
	*/ 
	auto ForwardThrow = FVector::DotProduct(TankForward, AIForwardIntention);
	IntendMoveForward(ForwardThrow);

	/*
	axb = |a| * |b| * sinθ * n, |a| and |b| are 1 because of GetSafeNormal, n should be (0, 0, 1)
	if we suppose Tank is only moving on the plain ground
	so the result of axb is (0, 0, sinθ * 1), then the Z value is sinθ
	sinθ is positive when θ = 0-180, which means should turn right
	sinθ is negative when θ = 180-360, which means should turn left
	the absolute value of cosθ is only for the strength of turning right
	*/
	auto RightThrow = FVector::CrossProduct(TankForward, AIForwardIntention).Z;
	IntendTurnRight(RightThrow);

	/*UE_LOG(LogTemp, Warning, 
		TEXT("Tank name: %s, TankForward: %s, AIForward: %s, ForwardThrow: %f, RightThrow: %f"), 
		*GetOwner()->GetName(), *TankForward.ToString(), *AIForwardIntention.ToString(), ForwardThrow, RightThrow);*/
}

void UTankMovementComponent::IntendMoveForward(float Throw)
{
	if (LeftTrack && RightTrack)
	{
		LeftTrack->SetThrottle(Throw);
		RightTrack->SetThrottle(Throw);
	}
}

void UTankMovementComponent::IntendTurnRight(float Throw)
{

	if (LeftTrack && RightTrack)
	{
		LeftTrack->SetThrottle(Throw);
		RightTrack->SetThrottle(-Throw);
	}
}