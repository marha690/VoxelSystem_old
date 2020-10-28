// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldSlice.h"
#include "ChunkData.h"
#include "WorldGenerator2.h"
#include "Structure.h"

#include "VoxFormatReader.h"
#include "TerrainNoise.h"

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
						chunk[i].setVoxel(VOXEL::VoxelData{ VOXEL::STONE, 0 }, x, y, (c % WORLD_PROPERTIES::VoxelsPerChunkDimension));
					else 
						chunk[i].setVoxel(VOXEL::VoxelData{ VOXEL::GRASS, 0 }, x, y, (c % WORLD_PROPERTIES::VoxelsPerChunkDimension));

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
			for (int x = 0; x < WORLD_PROPERTIES::VoxelsPerChunkDimension; x++)
				for (int y = 0; y < WORLD_PROPERTIES::VoxelsPerChunkDimension; y++)
				{
					for (size_t i = 0; i < Data.d[x][y].Num(); i++)
					{
						STRUCTURE::Data Voxel = Data.d[x][y][i];
						int cIndex = Voxel.height / WORLD_PROPERTIES::VoxelsPerChunkDimension;
						chunk[cIndex].setVoxel(Voxel.voxel, x, y, Voxel.height - cIndex * WORLD_PROPERTIES::VoxelsPerChunkDimension);
					}
				}
		}

	}
	else {
		//Trees
		if (!isObjectsGenerated) {
			auto v = VoxFormatReader("treeB.vox");
			int n = v.voxels.size();

			int xS = chash(SlicePositionIndex.X, SlicePositionIndex.Y, 123);
			int yS = chash(SlicePositionIndex.X, SlicePositionIndex.Y, 124);

			xS = (xS % 4) + 16;
			yS = (yS % 4) + 16;

			int startheight = UTerrainNoise::generate2DHeightMap(SlicePositionIndex.X * WORLD_PROPERTIES::VoxelsPerChunkDimension + xS,
																 SlicePositionIndex.Y * WORLD_PROPERTIES::VoxelsPerChunkDimension + yS);


			for (size_t i = 0; i < n; i++) {
				auto vox = v.voxels[i];
				int cIndex = (startheight + vox.first.Z) / WORLD_PROPERTIES::VoxelsPerChunkDimension;
				chunk[cIndex].setVoxel(VOXEL::VoxelData{ VOXEL::UNDETAILED, vox.second }, xS + vox.first.X, yS + vox.first.Y,
					vox.first.Z + startheight - cIndex * WORLD_PROPERTIES::VoxelsPerChunkDimension);
			}
			isObjectsGenerated = true;
		}

	}

}

void AWorldSlice::GenerateObjects()
{
	//if (!isObjectsGenerated) {
	//	auto v = VoxFormatReader("test.vox");
	//	int n = v.voxels.size();

	//	int startheight = UTerrainNoise::generate2DHeightMap(SlicePositionIndex.X * WORLD_PROPERTIES::VoxelsPerChunkDimension,
	//		SlicePositionIndex.Y * WORLD_PROPERTIES::VoxelsPerChunkDimension);

	//	int cIndex = startheight / WORLD_PROPERTIES::VoxelsPerChunkDimension;

	//	for (size_t i = 0; i < n; i++) {
	//		auto vox = v.voxels[i];
	//		chunk[cIndex].setVoxel(VOXEL::VoxelData{ VOXEL::UNDETAILED, vox.second }, 16 + vox.first.X, 16 + vox.first.Y, vox.first.Z + startheight - cIndex * WORLD_PROPERTIES::VoxelsPerChunkDimension);
	//	}
	//	isObjectsGenerated = true;
	//}
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
