// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Chunk.generated.h"

class AShapes;
class AtestActor;

/**
 * 
 */
UCLASS()
class VOXELWORLD_API AChunk : public AActor
{
	GENERATED_BODY()

public:

	//UPROPERTY(EditAnywhere)
		//TSubclassOf<AActor> ActorToSpawn;
	int _maxNumbeOfVoxels = 0;

	AChunk();
	void Initialize(FVector cIndex, int sizeXY, int sizeZ, AtestActor* _world);

	void BuildChunk();
	void RenderChunk();
	virtual void PostActorCreated() override;

protected:
	/* The data for the mesh for the chunk.*/
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FLinearColor> VertexColors;
	AShapes* voxels;
	AtestActor* world;

	FVector chunkIndex;
	int SizeXY;
	int SizeZ;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UProceduralMeshComponent* CustomMesh;

	virtual void PostLoad() override;
	virtual void BeginPlay() override;
	bool hasSolidNeighbour(int x, int y, int z);
	int ConvertVoxelToLocalXY(int i);
	int ConvertVoxelToLocalZ(int i);
public:
};