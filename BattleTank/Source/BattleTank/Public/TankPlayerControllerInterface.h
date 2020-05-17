// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class BATTLETANK_API TankPlayerControllerInterface
{
public:
	TankPlayerControllerInterface();
	~TankPlayerControllerInterface();

protected:
	virtual void OnPossessedTankDeath() = 0;
};
