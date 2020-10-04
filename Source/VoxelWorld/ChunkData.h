// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldProperties.h"

namespace VOXEL {
	
	const static enum BlockType {
		AIR = 0,
		BLOCKADE,
		STONE,
		GRASS,
		WOOD,
		LEAVES,
		TREESTART,
	};
}
using namespace VOXEL;

class AWorldSlice;
/**
 * 
 */
class VOXELWORLD_API ChunkData
{
public:
	const static enum Cubeside { BOTTOM, TOP, LEFT, RIGHT, FRONT, BACK };

	ChunkData();
	ChunkData(AWorldSlice* Owner, int Z);
	~ChunkData();

	AWorldSlice* SliceAsOwner;
	int ZPos;

	// UVs
	static const FVector2D UV00;
	static const FVector2D UV10;
	static const FVector2D UV01;
	static const FVector2D UV11;
	// Mesh variables
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FLinearColor> VertexColors;
	TArray<FVector> Normals;
	TArray<FVector2D> UV0;
	
	//Rendering
	void UpdateMeshData();
	void CreateQuad(Cubeside side, FVector indexInChunk);
	void AddTriangle(int32 V1, int32 V2, int32 V3);

	//Voxel
	bool HasSolidNeighbour(int x, int y, int z);
	void setVoxel(VOXEL::BlockType v, int x, int y, int z);
	BlockType& getVoxel(int x, int y, int z);
	int ConvertVoxelToLocal(int i);
	int linearIndex(int x, int y, int z);
	bool IsSolid(BlockType v);

private:
	VOXEL::BlockType bType[WORLD_PROPERTIES::VoxelsInChunk]{ VOXEL::AIR }; //data in chunk
	BlockType blockade = VOXEL::BLOCKADE;
};
