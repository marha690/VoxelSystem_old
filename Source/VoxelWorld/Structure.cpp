// Fill out your copyright notice in the Description page of Project Settings.


#include "Structure.h"
#include "TerrainNoise.h"
#include "VoxFormatReader.h"

#include "TerrainNoise.h"

Structure::Structure(StructureType T, int chunkDistance, FVector2D globalIndex)
{
	if (T != StructureType::None) {
		isStructure = true;
		ChunksDimension = chunkDistance;
		BlocksDimension = ChunksDimension * WORLD_PROPERTIES::VoxelsPerChunkDimension;
		Type = T;
		GlobalIndex = globalIndex;
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

void Structure::SetVoxel(VOXEL::VoxelData voxel, int X, int Y, int Z) {
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
	auto v = VoxFormatReader("testHouse.vox");
	int n = v.voxels.size();

	int startheight = UTerrainNoise::generate2DHeightMap(GlobalIndex.X * WORLD_PROPERTIES::VoxelsPerChunkDimension + 50, GlobalIndex.Y * WORLD_PROPERTIES::VoxelsPerChunkDimension + 50);
	for (size_t i = 0; i < n; i++) {
		auto vox = v.voxels[i];
		SetVoxel({ BlockType::UNDETAILED, vox.second }, 50 - vox.first.X, 50 - vox.first.Y, startheight + vox.first.Z);
	}

	startheight = UTerrainNoise::generate2DHeightMap(GlobalIndex.X * WORLD_PROPERTIES::VoxelsPerChunkDimension + 80, GlobalIndex.Y * WORLD_PROPERTIES::VoxelsPerChunkDimension + 50);
	for (size_t i = 0; i < n; i++) {
		auto vox = v.voxels[i];
		SetVoxel({ BlockType::UNDETAILED, vox.second }, 80 + vox.first.X, 50 + vox.first.Y, startheight + vox.first.Z);
	}

	startheight = UTerrainNoise::generate2DHeightMap(GlobalIndex.X * WORLD_PROPERTIES::VoxelsPerChunkDimension + 70, GlobalIndex.Y * WORLD_PROPERTIES::VoxelsPerChunkDimension + 70);
	for (size_t i = 0; i < n; i++) {
		auto vox = v.voxels[i];
		SetVoxel({ BlockType::UNDETAILED, vox.second }, 70 - vox.first.Y, 70 + vox.first.X, startheight + vox.first.Z);
	}

	////Borders of the village!
	//for (size_t x = 0; x < BlocksDimension; x++)
	//{
	//	int h = UTerrainNoise::generate2DHeightMap(x, 0);

	//	SetVoxel({ BlockType::STONE, 0 }, x, BlocksDimension - 1, 80);
	//	SetVoxel({ BlockType::STONE, 0 }, x, BlocksDimension - 2, 80);
	//	SetVoxel({ BlockType::STONE, 0 }, x, 0, 80);
	//	SetVoxel({ BlockType::STONE, 0 }, x, 1, 80);
	//}
	//for (size_t y = 0; y < BlocksDimension; y++)
	//{
	//	SetVoxel({ BlockType::STONE, 0 }, BlocksDimension - 1, y, 80);
	//	SetVoxel({ BlockType::STONE, 0 }, BlocksDimension - 2, y, 80);
	//	SetVoxel({ BlockType::STONE, 0 }, 0, y, 80);
	//	SetVoxel({ BlockType::STONE, 0 }, 1, y, 80);
	//}
}