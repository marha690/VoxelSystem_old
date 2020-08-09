// Fill out your copyright notice in the Description page of Project Settings.


#include "Chunk.h"
#include "Engine/World.h"
#include "Voxel.h"
#include "FastNoise.h"
#include "WorldGenerator.h"

AChunk::AChunk()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomMesh = CreateDefaultSubobject<UProceduralMeshComponent>("CustomMesh");
	SetRootComponent(CustomMesh);
	CustomMesh->bUseAsyncCooking = true;
}

void AChunk::Initialize(FVector cIndex, int size, AWorldGenerator*_world, UMaterial *mat)
{
	UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(mat, this);
	CustomMesh->SetMaterial(0, DynMaterial);
	chunkIndex = cIndex;
	Size = size;
	world = _world;
	status = ChunkStatus::GENERATING;
	NumberOfVoxels = Size * Size * Size;
	BuildChunk();
}

void AChunk::BuildChunk()
{
	(new FAutoDeleteAsyncTask<ChunkTask>(this))->StartBackgroundTask(); //Thread.
}

void AChunk::RenderChunk()
{
	//Move to thread later.
	for (size_t i = 0; i < NumberOfVoxels; i++)
	{
		if (!voxels[i].isSolid) continue;

		auto& voxelPos = voxels[i].indexInChunk;
		if (!hasSolidNeighbour(voxelPos.X - 1, voxelPos.Y, voxelPos.Z))
			voxels[i].CreateQuad(AVoxel::BACK, &Vertices, &Triangles, &UV0, &Normals);
		if (!hasSolidNeighbour(voxelPos.X + 1, voxelPos.Y, voxelPos.Z))
			voxels[i].CreateQuad(AVoxel::FRONT, &Vertices, &Triangles, &UV0, &Normals);
		if (!hasSolidNeighbour(voxelPos.X, voxelPos.Y - 1, voxelPos.Z))
			voxels[i].CreateQuad(AVoxel::LEFT, &Vertices, &Triangles, &UV0, &Normals);
		if (!hasSolidNeighbour(voxelPos.X, voxelPos.Y + 1, voxelPos.Z))
			voxels[i].CreateQuad(AVoxel::RIGHT, &Vertices, &Triangles, &UV0, &Normals);
		if (!hasSolidNeighbour(voxelPos.X, voxelPos.Y, voxelPos.Z + 1))
			voxels[i].CreateQuad(AVoxel::TOP, &Vertices, &Triangles, &UV0, &Normals);
		if (!hasSolidNeighbour(voxelPos.X, voxelPos.Y, voxelPos.Z - 1))
			voxels[i].CreateQuad(AVoxel::BOTTOM, &Vertices, &Triangles, &UV0, &Normals);
	}

	CustomMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true); // Texture.
}

bool AChunk::hasSolidNeighbour(int x, int y, int z)
{
	int yMult = Size;
	int zMult = Size * Size;

	FVector chunkOffset = FVector(0, 0, 0);
	if (x < 0) {
		chunkOffset -= FVector(1, 0, 0);
		x = ConvertVoxelToLocal(x);
	}
	else if (x > Size - 1) {
		chunkOffset += FVector(1, 0, 0);
		x = ConvertVoxelToLocal(x);
	}

	if (y < 0) {
		chunkOffset -= FVector(0, 1, 0);
		y = ConvertVoxelToLocal(y);
	}
	else if (y > Size - 1) {
		chunkOffset += FVector(0, 1, 0);
		y = ConvertVoxelToLocal(y);
	}

	if (z < 0) {
		chunkOffset -= FVector(0, 0, 1);
		z = ConvertVoxelToLocal(z);
	}
	else if (z > Size - 1) {
		chunkOffset += FVector(0, 0, 1);
		z = ConvertVoxelToLocal(z);
	}

	if (chunkOffset == FVector(0, 0, 0)) {
		//Inside this chunk.
		int listIndexValue = x + y * yMult + z * zMult;
		auto& neighbouringVoxel = voxels[listIndexValue];
		return neighbouringVoxel.isSolid;
	}
	else {
		//Inside other chunk.
		for (auto& c : world->chunks) {
			if (c->chunkIndex == (chunkIndex + chunkOffset) && c->status != c->ChunkStatus::GENERATING) {
				int listIndexValue = x + y * yMult + z * zMult;
				return c->voxels[listIndexValue].isSolid;
			}
		}
		return false; //Outside all read chunks.
	}
}

int AChunk::ConvertVoxelToLocal(int i)
{
	if (i == -1)
	{
		i = Size - 1;
	}
	else if (i == Size)
	{
		i = 0;
	}
	return i;
}

/******************
***	ChunkTask	***
******************/
ChunkTask::ChunkTask(AChunk* c)
	: chunk(c) {}

void ChunkTask::DoWork()
{
	int counter = 0;
	chunk->voxels = new AVoxel[chunk->NumberOfVoxels];

	//Set all voxels inside the chunk.
	for (int Z = 0; Z < chunk->Size; Z++) {
		for (int Y = 0; Y < chunk->Size; Y++) {
			for (int X = 0; X < chunk->Size; X++) {
				FVector index = FVector(X, Y, Z);
				FVector worldIndex = index + chunk->chunkIndex * FVector(chunk->Size, chunk->Size, chunk->Size);

				//MOVE THIS TO OTHER FILE
				FastNoise f(23);
				f.SetInterp(FastNoise::Interp::Linear);

				float adjust = 2.0f;
				auto val = abs(f.GetPerlin(worldIndex.X * adjust, worldIndex.Y * adjust)) * 16.0;

				if (worldIndex.Z > val) {
					chunk->voxels[counter] = AVoxel(AVoxel::AIR, index, counter);
				}
				else {
					chunk->voxels[counter] = AVoxel(AVoxel::STONE, index, counter);
				}
				//END OF MOVE THIS TO OTHER FILE

				counter++;
			}
		}
	}

	//Does not work in thread.
	//for (size_t i = 0; i < chunk->NumberOfVoxels; i++)
	//{
	//	if (!chunk->voxels[i].isSolid) continue;

	//	auto& voxelPos = chunk->voxels[i].indexInChunk;
	//	if (!chunk->hasSolidNeighbour(voxelPos.X - 1, voxelPos.Y, voxelPos.Z))
	//		chunk->voxels[i].CreateQuad(AVoxel::BACK, &chunk->Vertices, &chunk->Triangles, &chunk->UV0, &chunk->Normals);
	//	if (!chunk->hasSolidNeighbour(voxelPos.X + 1, voxelPos.Y, voxelPos.Z))
	//		chunk->voxels[i].CreateQuad(AVoxel::FRONT, &chunk->Vertices, &chunk->Triangles, &chunk->UV0, &chunk->Normals);
	//	if (!chunk->hasSolidNeighbour(voxelPos.X, voxelPos.Y - 1, voxelPos.Z))
	//		chunk->voxels[i].CreateQuad(AVoxel::LEFT, &chunk->Vertices, &chunk->Triangles, &chunk->UV0, &chunk->Normals);
	//	if (!chunk->hasSolidNeighbour(voxelPos.X, voxelPos.Y + 1, voxelPos.Z))
	//		chunk->voxels[i].CreateQuad(AVoxel::RIGHT, &chunk->Vertices, &chunk->Triangles, &chunk->UV0, &chunk->Normals);
		//if (!chunk->hasSolidNeighbour(voxelPos.X, voxelPos.Y, voxelPos.Z + 1))
		//	chunk->voxels[i].CreateQuad(AVoxel::TOP, &chunk->Vertices, &chunk->Triangles, &chunk->UV0, &chunk->Normals);
		//if (!chunk->hasSolidNeighbour(voxelPos.X, voxelPos.Y, voxelPos.Z - 1))
		//	chunk->voxels[i].CreateQuad(AVoxel::BOTTOM, &chunk->Vertices, &chunk->Triangles, &chunk->UV0, &chunk->Normals);
	//}

	//Update status so that worldGenerator can draw the chunk.
	chunk->status = chunk->ChunkStatus::DRAW;
}

