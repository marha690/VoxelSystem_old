// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Chunk.generated.h"

class AShapes;

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
	int _currentNumberOfVoxels = 0;

	AChunk();

	void BuildChunk(int32 sizeXY, int32 sizeZ);

	// Called actor is dragged/spawned in the world.
	virtual void PostActorCreated() override;

	//Level opened.
protected:
	virtual void PostLoad() override;

	/* The vertices of the mesh */
	TArray<FVector> Vertices;

	/* The triangles of the mesh */
	TArray<int32> Triangles;

	TArray<FLinearColor> VertexColors;


	virtual void BeginPlay() override;

	AShapes* voxels;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UProceduralMeshComponent* CustomMesh;


	void RenderChunk();

public:
	// Called every frame
};