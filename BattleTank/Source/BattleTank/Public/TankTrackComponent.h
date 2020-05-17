// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTrackComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankTrackComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetThrottle(float Throttle);

	// Max force per track, in Newtons
	UPROPERTY(EditAnywhere)
	float TrackMaxDrivingForce = 60000000.f;

private:
	UTankTrackComponent();
	//virtual void BeginPlay() override;

	TArray<class ASprungWheel*> GetWheels() const;

	//void ApplySidewayForce();
	//void DriveTrack();
	void DriveTrack(float CurrentThrottle);
	
	/*UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);*/

	//float CurrentThrottle = 0.f;
};