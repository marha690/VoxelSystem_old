// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldSlice.h"
#include "ChunkData.h"
#include "WorldGenerator2.h"
#include "Structure.h"

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
	StructureData data = WorldAsOwner->GetStructureData(SlicePositionIndex);

	if (data.s.isStructure) {
		STRUCTURE::DD Data;
		bool sucess = data.s.GetChunkData(SlicePositionIndex, Data);
		if (sucess) {

			for (int x = 0; x < 32; x++)
				for (int y = 0; y < 32; y++)
				{
					for (size_t i = 0; i < Data.d[x][y].Num(); i++)
					{
						STRUCTURE::Data Voxel = Data.d[x][y][i];
						int cIndex = Voxel.height / WORLD_PROPERTIES::VoxelsPerChunkDimension;
						chunk[cIndex].setVoxel(Voxel.voxel, x, y, Voxel.height - cIndex* WORLD_PROPERTIES::VoxelsPerChunkDimension);
					}
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

