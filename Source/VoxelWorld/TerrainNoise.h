// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TerrainNoise.generated.h"

/**
 * 
 */
UCLASS()
class VOXELWORLD_API UTerrainNoise : public UObject
{
	GENERATED_BODY()

public:
	static int generate2DHeightMap(int x, int y);
};
