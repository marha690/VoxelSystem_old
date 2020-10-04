// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldProperties.h"

namespace VOXEL {
	
	const static enum BlockType {
		AIR = 0,
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

	ChunkData(AWorldSlice* Owner);
	~ChunkData();

	VOXEL::BlockType bType[WORLD_PROPERTIES::VoxelsInChunk]{ VOXEL::STONE }; //data in chunk
	AWorldSlice* SliceAsOwner;

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


};
