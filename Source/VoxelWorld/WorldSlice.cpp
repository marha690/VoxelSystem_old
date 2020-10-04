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
		chunk[i].getVoxel(0, 0, 0) = VOXEL::STONE; //?
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

void AWorldSlice::GenerateTerrainFromNoise(int (*f)(int, int))
{
	for (int x = 0; x < WORLD_PROPERTIES::VoxelsPerChunkDimension; x++)
		for (int y = 0; y < WORLD_PROPERTIES::VoxelsPerChunkDimension; y++)
		{
			int h = f(x + SlicePositionIndex.X * WORLD_PROPERTIES::VoxelsPerChunkDimension,
					  y + SlicePositionIndex.Y * WORLD_PROPERTIES::VoxelsPerChunkDimension);
			for (int i = 0; i < WORLD_PROPERTIES::ChunksInHeight; i++) {
				int c = 0;
				while (h > c + i * WORLD_PROPERTIES::VoxelsPerChunkDimension && c < WORLD_PROPERTIES::VoxelsPerChunkDimension)
				{
					//chunk[i].getVoxel(x, y, (c % WORLD_PROPERTIES::VoxelsPerChunkDimension) ) = VOXEL::STONE;
					chunk[i].setVoxel(VOXEL::STONE, x, y, (c % WORLD_PROPERTIES::VoxelsPerChunkDimension));
					++c;
				}
			}
		}
}

void AWorldSlice::RenderChunks()
{
	for (int i = 0; i < WORLD_PROPERTIES::ChunksInHeight; i++)
	{
		chunk[i].UpdateMeshData();
		CustomMesh->CreateMeshSection_LinearColor(i, chunk[i].Vertices, chunk[i].Triangles, chunk[i].Normals, chunk[i].UV0, chunk[i].VertexColors, TArray<FProcMeshTangent>(), true);
	}
	isRendered = true;
}

