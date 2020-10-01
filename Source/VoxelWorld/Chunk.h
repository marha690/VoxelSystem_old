// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Chunk.generated.h"

class AVoxel;
class AWorldGenerator;

namespace VOXEL {
	const static enum Cubeside { BOTTOM, TOP, LEFT, RIGHT, FRONT, BACK };
	const static enum BlockType {
		AIR = 0,
		STONE,
		GRASS,
		WOOD,
		LEAVES,
		TREESTART,
	};
}

UCLASS()
class VOXELWORLD_API AChunk : public AActor
{
	GENERATED_BODY()

public:
	static enum ChunkStatus { LOAD, GENERATE, DRAW, DONE};
	ChunkStatus status;
	FVector chunkIndex;

	AChunk();
	void Initialize(FVector cIndex, AWorldGenerator* _world, UMaterial* mat, TArray<FColor> *colors);
	void BuildChunk();
	void RenderChunk();
	void generateStructures();

	static const uint8 Dimensions = 32;
	static const uint16 NumberOfVoxels = Dimensions* Dimensions* Dimensions;
	static const int32 voxelSize = 35;

	bool isSolid(VOXEL::BlockType b);
	VOXEL::BlockType& getVoxel(int x, int y, int z);
	int linearIndex(int x, int y, int z);
	bool hasSolidNeighbour(int x, int y, int z);
	FVector getVoxelWorldPosition(FVector pos);

private:
	VOXEL::BlockType bType[NumberOfVoxels]{ VOXEL::AIR }; //data in chunk

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

	
	AWorldGenerator* world;
	TArray<FColor> *colorAtlas;
	UProceduralMeshComponent* CustomMesh;

	virtual void Destroyed();

	void buildTrees(int x, int y, int z);

	int ConvertVoxelToLocal(int i);

	//Rendering
	void AddTriangle(int32 V1, int32 V2, int32 V3);
	void CreateQuad(VOXEL::Cubeside side, FVector indexInChunk);

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