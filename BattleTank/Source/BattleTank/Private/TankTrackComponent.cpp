// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTrackComponent.h"
#include "Components/PrimitiveComponent.h"
#include "SprungWheel.h"
#include "SpawnPoint.h"

UTankTrackComponent::UTankTrackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

//void UTankTrackComponent::BeginPlay()
//{
//	Super::BeginPlay();
//
//	OnComponentHit.AddDynamic(this, &UTankTrackComponent::OnHit);
//}

TArray<ASprungWheel*> UTankTrackComponent::GetWheels() const
{
	TArray<ASprungWheel*> SpawnWheels;
	TArray<USceneComponent*> ChildrenComponents;
	GetChildrenComponents(true, ChildrenComponents);
	for (auto Child : ChildrenComponents)
	{
		auto SpawnPointChild = Cast<USpawnPoint>(Child);
		if (!SpawnPointChild) continue;
		auto SpawnedChild = SpawnPointChild->GetSpawnedActor();
		auto SprungWheel = Cast<ASprungWheel>(SpawnedChild);
		if (!SprungWheel) continue;
		SpawnWheels.Add(SprungWheel);
	}
	return SpawnWheels;
}

//void UTankTrackComponent::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
//{
//	//DriveTrack();
//	ApplySidewayForce();
//	//CurrentThrottle = 0;
//}

//void UTankTrackComponent::ApplySidewayForce()
//{
//	// piao yi
//	auto SlippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());
//	auto DeltaTime = GetWorld()->GetDeltaSeconds();
//	auto CorrectionAcceleration = -SlippageSpeed / DeltaTime * GetRightVector();  // speend / time = acceleration
//	auto TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
//	auto CorrectForce = TankRoot->GetMass() * CorrectionAcceleration / 2; // Two tracks
//	TankRoot->AddForce(CorrectForce);
//}

void UTankTrackComponent::SetThrottle(float Throttle)
{
	float CurrentThrottle = FMath::Clamp(Throttle, -1.f, 1.f);
	DriveTrack(CurrentThrottle);
	//CurrentThrottle = 0;
}

//void UTankTrackComponent::DriveTrack()
//{
//	auto ForceApplied = GetForwardVector() * CurrentThrottle * TrackMaxDrivingForce;
//	auto ForceLocation = GetComponentLocation();
//	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
//	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
//}

void UTankTrackComponent::DriveTrack(float CurrentThrottle)
{
	auto ForceApplied = CurrentThrottle * TrackMaxDrivingForce;
	auto Wheels = GetWheels();
	auto ForcePerWheel = ForceApplied / Wheels.Num();
	for (ASprungWheel* Wheel : Wheels)
	{
		Wheel->AddDrivingForce(ForcePerWheel);
	}
}