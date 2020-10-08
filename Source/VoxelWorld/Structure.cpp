// Fill out your copyright notice in the Description page of Project Settings.


#include "Structure.h"

Structure::Structure(StructureType T, int chunkDistance)
{
	if (T != StructureType::None) {
		isStructure = true;
		ChunksDimension = chunkDistance;
		BlocksDimension = ChunksDimension * WORLD_PROPERTIES::VoxelsPerChunkDimension;
		Type = T;
		Generate();
	}
}

Structure::~Structure()
{
}

bool Structure::GetChunkData(FVector2D Index, DD &ChunkData)
{
	FVector2D localIndex = FVector2D((int)Index.X % ChunksDimension, (int)Index.Y % ChunksDimension);
	if (Chunks.Contains(localIndex)) {
		ChunkData = Chunks[localIndex];
		return true;
	}
	return false;
}

void Structure::Generate()
{
	switch (Type)
	{
	case None:
		break;
	case Village:
		GenerateVillage();
		break;
	case Tower:
		break;
	case Cabin:
		break;
	case Num_Structures:
		break;
	default:
		break;
	}
}

void Structure::SetVoxel(VOXEL::BlockType voxel, int X, int Y, int Z) {
	int _X = X / WORLD_PROPERTIES::VoxelsPerChunkDimension;
	int _Y = Y / WORLD_PROPERTIES::VoxelsPerChunkDimension;

	auto index = FVector2D(_X, _Y);
	if (Chunks.Contains(index)) {
		Chunks[index].d[X - _X * WORLD_PROPERTIES::VoxelsPerChunkDimension][Y - _Y * WORLD_PROPERTIES::VoxelsPerChunkDimension].Add(Data{ Z, voxel });
		//Chunks[index].d[X][Y].Add(Data{ Z, voxel });
	}
	else {
		DD data{};
		data.d[X - _X * WORLD_PROPERTIES::VoxelsPerChunkDimension][Y - _Y * WORLD_PROPERTIES::VoxelsPerChunkDimension].Add(Data{ Z, voxel });
		//data.d[X][Y].Add(Data{ Z, voxel });
		Chunks.Add(index, data);
	}

}

void Structure::GenerateVillage()
{
	for (size_t x = 0; x < 62; x++)
	{
		for (size_t y = 0; y < 62; y++)
		{
			SetVoxel(VOXEL::STONE, x, y, 71);
		}
	}
}
