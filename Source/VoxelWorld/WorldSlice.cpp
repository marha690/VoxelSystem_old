// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldSlice.h"
#include "ChunkData.h"

// Sets default values
AWorldSlice::AWorldSlice()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CustomMesh = CreateDefaultSubobject<UProceduralMeshComponent>("CustomMesh");
	SetRootComponent(CustomMesh);
	CustomMesh->bUseAsyncCooking = true;

	for (int i = 0; i < WORLD_PROPERTIES::ChunksInHeight; i++)
	{
		//Set correct values to the chunks.
		chunk[i].SliceAsOwner = this;
		chunk[i].ZPos = i;
		chunk[i].getVoxel(10, 10, 10) = VOXEL::STONE;
		chunk[i].getVoxel(10, 11, 10) = VOXEL::STONE;
		chunk[i].getVoxel(11, 11, 10) = VOXEL::STONE;

		chunk[i].getVoxel(3, 3, 0) = VOXEL::STONE;
		chunk[i].getVoxel(3, 3, 31) = VOXEL::STONE;

	}
}

void AWorldSlice::Initialize(FVector2D SPI, AWorldGenerator2* WRLD)
{
	SlicePositionIndex = SPI;
	WorldAsOwner = WRLD;
}

// Called when the game starts or when spawned
void AWorldSlice::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWorldSlice::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWorldSlice::RenderChunks()
{
	for (int i = 0; i < WORLD_PROPERTIES::ChunksInHeight; i++)
	{
		chunk[i].UpdateMeshData();
		CustomMesh->CreateMeshSection_LinearColor(i, chunk[i].Vertices, chunk[i].Triangles, chunk[i].Normals, chunk[i].UV0, chunk[i].VertexColors, TArray<FProcMeshTangent>(), true);
	}
}

