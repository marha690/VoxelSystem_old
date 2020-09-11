// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class VOXELWORLD_API WorldNoise
{
public:

	static int getGroundHeight(int x, int y);
private:
	WorldNoise() {};
};
