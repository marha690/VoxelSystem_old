// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ChunkData.h"
#include "Structure.h"

struct StructureData {
	StructureType Type;
	Structure s;
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
	int LevelSize = 4;
	int seed = 123;
	TMap<FVector2D, StructureData> DataInSlice;
	
	StructureData GenerateData(FVector2D Index);
	StructureType GetStructureType(FVector2D Index);

	int chash(int x, int y);
};


