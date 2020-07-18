// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


class VOXELWORLD_API AShapes
{

public:
	static enum Cubeside {
		BOTTOM,
		TOP,
		LEFT,
		RIGHT,
		FRONT,
		BACK
	};
	static enum BlockType {
		GRASS,
		STONE
	};

	// Sets default values for this actor's properties
	AShapes();
	AShapes(BlockType b, FVector pos, int index);

	FVector indexInChunk;
	int listIndex;

	void GenerateCubeMesh(TArray<FVector>* vertices, TArray<FLinearColor>* vertexColors);
	void Draw(TArray<int32> *triangles);
protected:
	BlockType bType;

	/* Creates a triangle that connects the given vertices */
	void AddTriangle(int32 V1, int32 V2, int32 V3, TArray<int32>* triangles);

	void CreateQuad(Cubeside side, TArray<int32>* triangles);

	int32 voxelSize = 200;
	int32 halfSize = voxelSize / 2;
private:

};
