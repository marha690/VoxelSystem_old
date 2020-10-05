// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldSlice.h"
#include "ChunkData.h"
#include "WorldGenerator2.h"

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
	}
}

void AWorldSlice::Initialize(FVector2D SPI, AWorldGenerator2* WRLD)
{
	SlicePositionIndex = SPI;
	WorldAsOwner = WRLD;

	UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(WorldAsOwner->material, this);
	for (int i = 0; i < WORLD_PROPERTIES::ChunksInHeight; i++)
		CustomMesh->SetMaterial(i, DynMaterial);
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
					if (h > c + 3)
						chunk[i].setVoxel(VOXEL::STONE, x, y, (c % WORLD_PROPERTIES::VoxelsPerChunkDimension));
					else 
						chunk[i].setVoxel(VOXEL::GRASS, x, y, (c % WORLD_PROPERTIES::VoxelsPerChunkDimension));


					++c;
				}
			}
		}
}

void AWorldSlice::GenerateStructures()
{
	auto data = WorldAsOwner->GetStructureData(SlicePositionIndex);

	if (data.Type == StructureType::Village) {
		for (int i = 0; i < WORLD_PROPERTIES::ChunksInHeight; i++) {
			chunk[i].setVoxel(VOXEL::STONE, 10, 10, 10);
			chunk[i].setVoxel(VOXEL::STONE, 10, 9, 10);
			chunk[i].setVoxel(VOXEL::STONE, 10, 11, 10);

			chunk[i].setVoxel(VOXEL::STONE, 10, 9, 11);
			chunk[i].setVoxel(VOXEL::STONE, 10, 11, 11);
			chunk[i].setVoxel(VOXEL::STONE, 10, 9, 12);
			chunk[i].setVoxel(VOXEL::STONE, 10, 11, 12);
			chunk[i].setVoxel(VOXEL::STONE, 10, 9, 13);
			chunk[i].setVoxel(VOXEL::STONE, 10, 11, 13);

			chunk[i].setVoxel(VOXEL::STONE, 10, 10, 14);
			chunk[i].setVoxel(VOXEL::STONE, 10, 9, 14);
			chunk[i].setVoxel(VOXEL::STONE, 10, 11, 14);
		}
	}

	if (data.Type == StructureType::Tower) {
		for (int i = 0; i < WORLD_PROPERTIES::ChunksInHeight; i++) {
			chunk[i].setVoxel(VOXEL::GRASS, 0, 10, 2);
			chunk[i].setVoxel(VOXEL::GRASS, 0, 10, 3);
			chunk[i].setVoxel(VOXEL::GRASS, 0, 10, 4);
			chunk[i].setVoxel(VOXEL::GRASS, 0, 10, 5);
			chunk[i].setVoxel(VOXEL::GRASS, 0, 10, 6);
			chunk[i].setVoxel(VOXEL::GRASS, 0, 9, 5);
		}
	}

	if (data.Type == StructureType::Cabin) {

			for (int x = 0; x < 32; x++)
				for (int y = 0; y < 32; y++)
				{
					chunk[6].setVoxel(VOXEL::STONE, x, y, 0);
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

