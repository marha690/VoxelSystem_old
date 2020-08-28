// Fill out your copyright notice in the Description page of Project Settings.


#include "Chunk.h"
#include "Engine/World.h"
#include "Voxel.h"
#include "WorldNoise.h"
#include "WorldGenerator.h"

AChunk::AChunk()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomMesh = CreateDefaultSubobject<UProceduralMeshComponent>("CustomMesh");
	SetRootComponent(CustomMesh);
	CustomMesh->bUseAsyncCooking = true;
}

void AChunk::Initialize(FVector cIndex, AWorldGenerator*_world, UMaterial *mat, FColor* a)
{
	UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(mat, this);
	CustomMesh->SetMaterial(0, DynMaterial);
	chunkIndex = cIndex;
	world = _world;
	status = ChunkStatus::LOAD;
	voxels = new AVoxel[NumberOfVoxels];
	atlas = a;
	BuildChunk();
}

void AChunk::BuildChunk()
{
	(new FAutoDeleteAsyncTask<ChunkTask>(this))->StartBackgroundTask(); //Thread.
}

void AChunk::generateStructures() {
	//Trees
	if (!featuresCreated)
	{
		for (size_t i = 0; i < Dimensions; i++)
		{
			for (size_t j = 0; j < Dimensions; j++)
			{
				for (size_t k = 0; k < Dimensions; k++)
				{
					buildTrees(i, j, k);
				}
			}
		}
		featuresCreated = true;
	}
	status = ChunkStatus::DRAW;
}

void AChunk::RenderChunk()
{
	//Move to thread later.
	for (size_t i = 0; i < NumberOfVoxels; i++)
	{
		if (!voxels[i].isSolid()) continue;

		auto& voxelPos = voxels[i].indexInChunk;
		if (!hasSolidNeighbour(voxelPos.X - 1, voxelPos.Y, voxelPos.Z))
			voxels[i].CreateQuad(AVoxel::BACK, &Vertices, &Triangles, &UV0, &Normals, &VertexColors);
		if (!hasSolidNeighbour(voxelPos.X + 1, voxelPos.Y, voxelPos.Z))
			voxels[i].CreateQuad(AVoxel::FRONT, &Vertices, &Triangles, &UV0, &Normals, &VertexColors);
		if (!hasSolidNeighbour(voxelPos.X, voxelPos.Y - 1, voxelPos.Z))
			voxels[i].CreateQuad(AVoxel::LEFT, &Vertices, &Triangles, &UV0, &Normals, &VertexColors);
		if (!hasSolidNeighbour(voxelPos.X, voxelPos.Y + 1, voxelPos.Z))
			voxels[i].CreateQuad(AVoxel::RIGHT, &Vertices, &Triangles, &UV0, &Normals, &VertexColors);
		if (!hasSolidNeighbour(voxelPos.X, voxelPos.Y, voxelPos.Z + 1))
			voxels[i].CreateQuad(AVoxel::TOP, &Vertices, &Triangles, &UV0, &Normals, &VertexColors);
		if (!hasSolidNeighbour(voxelPos.X, voxelPos.Y, voxelPos.Z - 1))
			voxels[i].CreateQuad(AVoxel::BOTTOM, &Vertices, &Triangles, &UV0, &Normals, &VertexColors);
	}

	CustomMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, VertexColors, TArray<FProcMeshTangent>(), true); // Texture.
	status = ChunkStatus::DONE;
}

void AChunk::ReRenderChunk()
{
	Vertices.Empty();
	UV0.Empty();
	Normals.Empty();
	VertexColors.Empty();
	featuresCreated = false;
	RenderChunk();
}

void AChunk::Destroyed()
{
	delete[] voxels;
}

void AChunk::buildTrees(int x, int y, int z)
{
	if (getVoxel(x, y, z)->bType != AVoxel::TREESTART)
		return;

	AVoxel* t = getVoxel(x, y, z + 1);

	if (t != nullptr) {
		t->setVoxelType(AVoxel::WOOD);
	}

	t = getVoxel(x, y, z + 2);
	if (t != nullptr) {
		t->setVoxelType(AVoxel::WOOD);
	}

	//First floor.
	for (int i = -2; i <= 2; i++)
	{
		for (int j = -2; j <= 2; j++)
		{
			if ((i == j || i == -j) && (i == 2 || i == -2) )
				continue;

			t = getVoxel(x + i, y + j, z + 3);
			if (t != nullptr) {
				t->setVoxelType(AVoxel::LEAVES);
			}
		}
	}

	t = getVoxel(x, y, z + 4);
	if (t != nullptr) {
		t->setVoxelType(AVoxel::WOOD);
	}
	t = getVoxel(x, y, z + 5);
	if (t != nullptr) {
		t->setVoxelType(AVoxel::WOOD);
	}

	//second floor.
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			t = getVoxel(x + i, y + j, z + 5);
			if (t != nullptr) {
				t->setVoxelType(AVoxel::LEAVES);
			}
		}
	}

	t = getVoxel(x, y, z + 6);
	if (t != nullptr) {
		t->setVoxelType(AVoxel::LEAVES);
	}

}

AVoxel* AChunk::getVoxel(int x, int y, int z)
{
	if (x >= Dimensions || y >= Dimensions || z >= Dimensions || x < 0 || y < 0 || z < 0) { // Inside other chunk.
		
		int newX = x;
		int newY = y;
		int newZ = z;

		FVector chunkOffset = FVector(0, 0, 0);
		if (x < 0) {
			chunkOffset -= FVector(1, 0, 0);
			x = ConvertVoxelToLocal(x);
		}
		else if (x > Dimensions - 1) {
			chunkOffset += FVector(1, 0, 0);
			x = ConvertVoxelToLocal(x);
		}

		if (y < 0) {
			chunkOffset -= FVector(0, 1, 0);
			y = ConvertVoxelToLocal(y);
		}
		else if (y > Dimensions - 1) {
			chunkOffset += FVector(0, 1, 0);
			y = ConvertVoxelToLocal(y);
		}

		if (z < 0) {
			chunkOffset -= FVector(0, 0, 1);
			z = ConvertVoxelToLocal(z);
		}
		else if (z > Dimensions - 1) {
			chunkOffset += FVector(0, 0, 1);
			z = ConvertVoxelToLocal(z);
		}
		FVector newChunkIndex = chunkIndex + chunkOffset;

		for (auto& c : world->chunks) {

			if (c->chunkIndex == newChunkIndex && c->status != c->ChunkStatus::LOAD) {
				int listIndex = getVoxelIndex(x, y, z);
				return &c->voxels[listIndex];
			}
		}
		return nullptr;
	}

	// Inside chunk
	int index = getVoxelIndex(x, y, z);
	return &voxels[index];
}

int AChunk::getVoxelIndex(int x, int y, int z)
{
	return x + y * Dimensions + z * Dimensions * Dimensions;
}

bool AChunk::hasSolidNeighbour(int x, int y, int z)
{
	FVector chunkOffset = FVector(0, 0, 0);
	if (x < 0) {
		chunkOffset -= FVector(1, 0, 0);
		x = ConvertVoxelToLocal(x);
	}
	else if (x > Dimensions - 1) {
		chunkOffset += FVector(1, 0, 0);
		x = ConvertVoxelToLocal(x);
	}

	if (y < 0) {
		chunkOffset -= FVector(0, 1, 0);
		y = ConvertVoxelToLocal(y);
	}
	else if (y > Dimensions - 1) {
		chunkOffset += FVector(0, 1, 0);
		y = ConvertVoxelToLocal(y);
	}

	if (z < 0) {
		chunkOffset -= FVector(0, 0, 1);
		z = ConvertVoxelToLocal(z);
	}
	else if (z > Dimensions - 1) {
		chunkOffset += FVector(0, 0, 1);
		z = ConvertVoxelToLocal(z);
	}

	if (chunkOffset == FVector(0, 0, 0)) {
		//Inside this chunk.
		int listIndex = getVoxelIndex(x, y, z);
		auto& neighbouringVoxel = voxels[listIndex];
		return neighbouringVoxel.isSolid();
	}
	else {
		//Inside other chunk.
		for (auto& c : world->chunks) {
			if (c->chunkIndex == (chunkIndex + chunkOffset) && c->status != c->ChunkStatus::LOAD) {

				int listIndex = getVoxelIndex(x, y, z);
				return c->voxels[listIndex].isSolid();
			}
		}
		return false; //Outside all read chunks.
	}
}

int AChunk::ConvertVoxelToLocal(int i)
{
	if (i <= -1) {
		i = Dimensions + i;
	}
	else if (i >= Dimensions) {
		i = i - Dimensions;
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
	//Set all voxels inside the chunk.
	for (int Z = 0; Z < chunk->Dimensions; Z++) {
		for (int Y = 0; Y < chunk->Dimensions; Y++) {
			for (int X = 0; X < chunk->Dimensions; X++) {
				FVector localIndex = FVector(X, Y, Z);
				FVector worldIndex = localIndex + chunk->chunkIndex * FVector(chunk->Dimensions, chunk->Dimensions, chunk->Dimensions);
				WorldNoise::run(chunk->voxels[counter], worldIndex, localIndex, counter, chunk);
				counter++;
			}
		}
	}

	//Update status so that worldGenerator can draw the chunk.
	chunk->status = chunk->ChunkStatus::GENERATE;
}

