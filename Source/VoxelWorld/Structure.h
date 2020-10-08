// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChunkData.h"


enum StructureType
{
	None = 0,
	Village,
	Tower,
	Cabin,

	Num_Structures
};

namespace STRUCTURE {
	struct Data { int height; VOXEL::BlockType voxel; };
	struct DD { TArray<Data> d[WORLD_PROPERTIES::VoxelsPerChunkDimension][WORLD_PROPERTIES::VoxelsPerChunkDimension]; };
}
using namespace STRUCTURE;

class Structure
{
private:
public:
	Structure(StructureType T, int ChunkDistance);
	~Structure();
	bool GetChunkData(FVector2D Index, DD& ChunkData);

	bool isStructure = false;
private:
	void Generate();
	void SetVoxel(VOXEL::BlockType voxel, int X, int Y, int Z);
	void GenerateVillage();

	StructureType Type;
	int ChunksDimension = 0;
	int BlocksDimension = 0;

	TMap < FVector2D, DD> Chunks;
};


