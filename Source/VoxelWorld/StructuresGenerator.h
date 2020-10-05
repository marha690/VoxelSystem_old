// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
enum LevelType {
	One,
	Two,
	Three
};

enum StructureType
{
	None = 0,
	Occupied,
	Village,
	Tower,
	Cabin,

	Num_Structures
};

struct StructureData {
	StructureType Type;
};

class VOXELWORLD_API StructuresGenerator
{
public:
	StructuresGenerator();
	StructuresGenerator(int RenderDist);
	~StructuresGenerator();

	void GenerateArea(FVector2D PlayerPosition);
	StructureData GetData(FVector2D index) { return DataInSlice[index]; }


private:
	int RenderDistance;

	int seed = 123;
	int Level1 = 16; // Largest things.
	int Level2 = 8;
	int Level3 = 4;

	TMap<FVector2D, struct StructureData> DataInSlice;

	StructureData GenerateData(FVector2D Index);
	StructureType GetStructureType(FVector2D Index, int DistanceLevel, LevelType t);
	void RemoveGenerationsInSlices();
	int chash(int x, int y);
};
