// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


class VOXELWORLD_API AShapes
{

public:
	static enum Cubeside { BOTTOM, TOP, LEFT, RIGHT, FRONT, BACK };
	static enum BlockType {
		AIR,
		STONE,
		GRASS
	};

	// Sets default values for this actor's properties
	AShapes();
	AShapes(BlockType b, FVector pos, int index);

	//Properites for the voxel.
	BlockType bType;
	FVector indexInChunk;
	int listIndex;
	bool isSolid;

	void GenerateCubeMesh(TArray<FVector>* vertices, TArray<FLinearColor>* vertexColors);
	void CreateQuad(Cubeside side, TArray<int32>* triangles);
protected:
	void AddTriangle(int32 V1, int32 V2, int32 V3, TArray<int32>* triangles);

	//Move these later
	int32 voxelSize = 200;
	int32 halfSize = voxelSize / 2;
private:
	void Draw(TArray<int32> *triangles); // No longer in use.
};
