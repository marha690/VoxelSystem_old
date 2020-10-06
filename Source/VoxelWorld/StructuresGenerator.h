// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

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

	const StructureData& GetData(FVector2D Index);
	void DeleteGenerationsInSlices(FVector2D PlayerAtSlice);


private:
	int RenderDistance;

	int seed = 123;
	int Level1Size = 16;
	int Level2Size= 8;
	int Level3Size = 4; 

	TMap<FVector2D, struct StructureData> DataInSlice;
	
	StructureData GenerateData(FVector2D Index);
	StructureType GetStructureType(FVector2D Index, int Distance, int Level);
	void GenerateArea(StructureData Data, FVector2D Index);

	int chash(int x, int y);
};
