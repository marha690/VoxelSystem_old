// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


class VOXELWORLD_API AVoxel
{

public:
	static enum Cubeside { BOTTOM, TOP, LEFT, RIGHT, FRONT, BACK };
	static enum BlockType {
		AIR,
		STONE,
		GRASS,
		TREESTART,
		LEAVES,
		WOOD,
	};

	static const int32 voxelSize = 50;

	AVoxel();
	AVoxel(BlockType b, FVector pos, int index);

	void setVoxelType(BlockType type);

	//Properites for the voxel.
	BlockType bType;
	FVector indexInChunk;
	int listIndex;
	bool isSolid;

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
	FVector2D UV00 = FVector2D(0.f, 0.f);
	FVector2D UV10 = FVector2D(1.f, 0.f);
	FVector2D UV01 = FVector2D(0.f, 1.f);
	FVector2D UV11 = FVector2D(1.f, 1.f);

	void CreateQuad(Cubeside side, TArray<FVector>* vertices, TArray<int32>* triangles, TArray<FVector2D>* uv, TArray<FVector> *normals, TArray<FLinearColor>* vertexColors);

	FLinearColor getVertexColor();
protected:
	void AddTriangle(int32 V1, int32 V2, int32 V3, TArray<int32>* triangles);
};
