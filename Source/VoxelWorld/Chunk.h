// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Chunk.generated.h"

class AVoxel;
class AWorldGenerator;

/**
 * 
 */
UCLASS()
class VOXELWORLD_API AChunk : public AActor
{
	GENERATED_BODY()

public:
	static enum ChunkStatus { DRAW, DONE, KEEP};
	ChunkStatus status;
	FVector chunkIndex;
	int _maxNumberOfVoxels = 0;

	AChunk();
	void Initialize(FVector cIndex, int sizeXY, int sizeZ, AWorldGenerator* _world);
	void BuildChunk();
	void RenderChunk();

protected:
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FLinearColor> VertexColors;
	AVoxel* voxels;
	AWorldGenerator* world;

	int SizeXY;
	int SizeZ;

	UProceduralMeshComponent* CustomMesh;

	bool hasSolidNeighbour(int x, int y, int z);
	int ConvertVoxelToLocalXY(int i);
	int ConvertVoxelToLocalZ(int i);
public:
};