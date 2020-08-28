// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class AChunk;

class VOXELWORLD_API AVoxel
{

public:
	const static enum Cubeside { BOTTOM, TOP, LEFT, RIGHT, FRONT, BACK };
	const static enum BlockType {
		AIR,
		STONE,
		GRASS,
		WOOD,
		LEAVES,
		TREESTART,
	};

	AVoxel();
	AVoxel(BlockType b, FVector pos, int index, AChunk* p);

	void setVoxelType(BlockType type);
	bool isSolid() { return solid; }
	static const int32 voxelSize = 50;

private:
	AChunk* parent;
	BlockType bType;
	FVector indexInChunk;
	uint16 listIndex;
	bool solid;

	// Vertices
	FVector p0;
	FVector p1;
	FVector p2;
	FVector p3;
	FVector p4;
	FVector p5;
	FVector p6;
	FVector p7;

	// UVs
	static const FVector2D UV00;
	static const FVector2D UV10;
	static const FVector2D UV01;
	static const FVector2D UV11;

	FLinearColor getVertexColor();
	void AddTriangle(int32 V1, int32 V2, int32 V3, TArray<int32>* triangles);
	void CreateQuad(Cubeside side, TArray<FVector>* vertices, TArray<int32>* triangles, TArray<FVector2D>* uv, TArray<FVector> *normals, TArray<FLinearColor>* vertexColors);

	friend class AChunk;

};



//class VertexColorManagement
//{
//public:
//	static FColor getColor(int index);
//	//static const UTexture2D* defaultAtlas;
//private:
//
//	VertexColorManagement();
//	~VertexColorManagement() {};
//};
//
//FColor AChunk::VertexColorManagement::getColor(int index)
//{
//	return FColor();
//	//return *static_cast<FColor*>(defaultAtlas->PlatformData->Mips[index].BulkData.Lock(LOCK_READ_ONLY));
//}
//
//VertexColorManagement::VertexColorManagement()
//{
//	//defaultAtlas = LoadObject<UTexture2D>(NULL, TEXT("/Game/VoxelVertexColorAtlas/Default.Default"), NULL, LOAD_None, NULL);
//}