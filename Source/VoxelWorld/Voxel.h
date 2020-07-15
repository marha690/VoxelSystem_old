// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Voxel.generated.h"


enum Cubeside {
	BOTTOM,
	TOP,
	LEFT,
	RIGHT,
	FRONT,
	BACK
};

enum BlockType {
	GRASS,
	STONE
};


UCLASS()
class VOXELWORLD_API AVoxel : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AVoxel();
	AVoxel(BlockType b, FVector pos);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called actor is dragged/spawned in the world.
	virtual void PostActorCreated() override;

	//Level opened.
	virtual void PostLoad() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UProceduralMeshComponent* CustomMesh;

	BlockType bType;
	FVector position;

	/* The vertices of the mesh */
	TArray<FVector> Vertices;

	/* The triangles of the mesh */
	TArray<int32> Triangles;

	TArray<FLinearColor> VertexColors;

	/* Creates a triangle that connects the given vertices */
	void AddTriangle(int32 V1, int32 V2, int32 V3);

	void CreateQuad(Cubeside side);

	void GenerateCubeMesh();
	void Draw();

	int32 voxelSize = 200;
	int32 halfSize = voxelSize / 2;
private:

};
