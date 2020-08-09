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
	static enum ChunkStatus { DRAW, DONE, KEEP, GENERATING};
	ChunkStatus status;
	FVector chunkIndex;

	AChunk();
	void Initialize(FVector cIndex, int size, AWorldGenerator* _world, UMaterial* mat);
	void BuildChunk();
	void RenderChunk();

private:
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FLinearColor> VertexColors;

	TArray<FVector> Normals;
	TArray<FVector2D> UV0;

	AVoxel* voxels;
	AWorldGenerator* world;

	int Size;
	int NumberOfVoxels;

	UProceduralMeshComponent* CustomMesh;

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