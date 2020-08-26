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
	static enum ChunkStatus { LOAD, GENERATE, DRAW, DONE};
	ChunkStatus status;
	FVector chunkIndex;

	AChunk();
	void Initialize(FVector cIndex, int size, AWorldGenerator* _world, UMaterial* mat);
	void BuildChunk();
	void RenderChunk();
	void ReRenderChunk();
	void generateStructures();

private:
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FLinearColor> VertexColors;

	TArray<FVector> Normals;
	TArray<FVector2D> UV0;

	AVoxel* voxels;
	AWorldGenerator* world;

	int Size; //voxels in each side 
	int NumberOfVoxels; // all voxels which exists inside the chunk.

	bool featuresCreated = false;

	UProceduralMeshComponent* CustomMesh;

	virtual void Destroyed();

	void buildTrees(int x, int y, int z);
	AVoxel* getVoxel(int x, int y, int z);
	int getVoxelIndex(int x, int y, int z);
	bool hasSolidNeighbour(int x, int y, int z);
	int ConvertVoxelToLocal(int i);
	friend class ChunkTask;
};

/******************
***	ChunkTask	***
******************/
class ChunkTask : public FNonAbandonableTask
{
public:
	ChunkTask(AChunk* c);

	FORCEINLINE TStatId GetStatId() const {
		RETURN_QUICK_DECLARE_CYCLE_STAT(ChunkTask, STATGROUP_ThreadPoolAsyncTasks);
	}
	void DoWork();

private:
	AChunk* chunk;
};