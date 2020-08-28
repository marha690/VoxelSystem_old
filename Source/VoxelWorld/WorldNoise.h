// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voxel.h"

class AChunk;

class VOXELWORLD_API WorldNoise
{
public:
	static void run(AVoxel& v, FVector position, FVector voxelLocalPos, int voxelIndex, AChunk* c);

private:
	static float getGroundHeight(FVector position);

	WorldNoise() {};
};
