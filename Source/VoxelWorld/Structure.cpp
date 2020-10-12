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
	int xMod = (int)Index.X % ChunksDimension;
	int yMod = (int)Index.Y % ChunksDimension;

	int lvX = 0;
	int lvY = 0;
	if (Index.X >= 0)
		lvX = xMod;
	else {
		lvX = 4 - abs(xMod);

		xMod = abs((int)Index.X) % ChunksDimension;
		lvX = 4 - xMod;
		if (lvX == 4) {
			lvX = 0;
		}
	}

	if (Index.Y >= 0)
		lvY = yMod;
	else {
		lvY = 4 - abs(yMod);

		yMod = abs((int)Index.Y) % ChunksDimension;
		lvY = 4 - yMod;
		if (lvY == 4) {
			lvY = 0;
		}
	}

	FVector2D indexSquish = FVector2D(lvX, lvY);

	if (Chunks.Contains(indexSquish)) {
		ChunkData = Chunks[indexSquish];
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
	}
	else {
		DD data{};
		data.d[X - _X * WORLD_PROPERTIES::VoxelsPerChunkDimension][Y - _Y * WORLD_PROPERTIES::VoxelsPerChunkDimension].Add(Data{ Z, voxel });
		Chunks.Add(index, data);
	}
}

void Structure::GenerateVillage()
{
	for (size_t x = 0; x < BlocksDimension; x++)
	{
		SetVoxel(VOXEL::STONE, x, BlocksDimension - 1, 80);
		SetVoxel(VOXEL::STONE, x, BlocksDimension - 2, 80);
		SetVoxel(VOXEL::STONE, x, 0, 80);
		SetVoxel(VOXEL::STONE, x, 1, 80);
	}
	for (size_t y = 0; y < BlocksDimension; y++)
	{
		SetVoxel(VOXEL::STONE, BlocksDimension - 1, y, 80);
		SetVoxel(VOXEL::STONE, BlocksDimension - 2, y, 80);
		SetVoxel(VOXEL::STONE, 0, y, 80);
		SetVoxel(VOXEL::STONE, 1, y, 80);
	}
}