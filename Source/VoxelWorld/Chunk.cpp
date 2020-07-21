// Fill out your copyright notice in the Description page of Project Settings.


#include "Chunk.h"
#include "Engine/World.h"
#include "Shapes.h"
#include "WorldGenerator.h"

AChunk::AChunk()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomMesh = CreateDefaultSubobject<UProceduralMeshComponent>("CustomMesh");
	SetRootComponent(CustomMesh);
	CustomMesh->bUseAsyncCooking = true;
}

void AChunk::Initialize(FVector cIndex, int sizeXY, int sizeZ, AWorldGenerator*_world)
{
	chunkIndex = cIndex;
	SizeXY = sizeXY;
	SizeZ = sizeZ;
	world = _world;
	BuildChunk();
}

void AChunk::BuildChunk()
{
	_maxNumbeOfVoxels = SizeXY * SizeXY * SizeZ;
	voxels = new AShapes[_maxNumbeOfVoxels];
	int c = 0;
	for (int Z = 0; Z < SizeZ; Z++) {
		for (int Y = 0; Y < SizeXY; Y++) {
			for (int X = 0; X < SizeXY; X++) {
				UWorld* WRLD = GetWorld();
				if (GetWorld()) {
					FVector index = FVector(X, Y, Z);
					if (Y < 2 && X < 2 && Z == 2) {
					voxels[c] = AShapes(AShapes::AIR, index, c);
					}
					else {
					voxels[c] = AShapes(AShapes::STONE, index, c);
					}

					voxels[c].GenerateCubeMesh(&Vertices, &VertexColors);
					c++;
				}
			}
		}
	}
}

void AChunk::RenderChunk()
{
	for (size_t i = 0; i < _maxNumbeOfVoxels; i++)
	{
		if (!voxels[i].isSolid) continue;

		auto& voxelPos = voxels[i].indexInChunk;
		if (!hasSolidNeighbour(voxelPos.X - 1, voxelPos.Y, voxelPos.Z))
			voxels[i].CreateQuad(AShapes::BACK, &Triangles);
		if (!hasSolidNeighbour(voxelPos.X + 1, voxelPos.Y, voxelPos.Z))
			voxels[i].CreateQuad(AShapes::FRONT, &Triangles);
		if (!hasSolidNeighbour(voxelPos.X, voxelPos.Y - 1, voxelPos.Z))
			voxels[i].CreateQuad(AShapes::LEFT, &Triangles);
		if (!hasSolidNeighbour(voxelPos.X, voxelPos.Y + 1, voxelPos.Z))
			voxels[i].CreateQuad(AShapes::RIGHT, &Triangles);
		if (!hasSolidNeighbour(voxelPos.X, voxelPos.Y, voxelPos.Z + 1))
			voxels[i].CreateQuad(AShapes::TOP, &Triangles);
		if (!hasSolidNeighbour(voxelPos.X, voxelPos.Y, voxelPos.Z - 1))
			voxels[i].CreateQuad(AShapes::BOTTOM, &Triangles);
	}
	CustomMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), VertexColors, TArray<FProcMeshTangent>(), true);
}

bool AChunk::hasSolidNeighbour(int x, int y, int z)
{
	int yMult = SizeXY;
	int zMult = SizeXY * SizeXY;

	FVector chunkOffset = FVector(0, 0, 0);
	if (x < 0) {
		chunkOffset -= FVector(1, 0, 0);
		x = ConvertVoxelToLocalXY(x);
	}
	else if (x > SizeXY - 1) {
		chunkOffset += FVector(1, 0, 0);
		x = ConvertVoxelToLocalXY(x);
	}

	if (y < 0) {
		chunkOffset -= FVector(0, 1, 0);
		y = ConvertVoxelToLocalXY(y);
	}
	else if (y > SizeXY - 1) {
		chunkOffset += FVector(0, 1, 0);
		y = ConvertVoxelToLocalXY(y);
	}

	if (z < 0) {
		chunkOffset -= FVector(0, 0, 1);
		z = ConvertVoxelToLocalZ(z);
	}
	else if (z > SizeZ - 1) {
		chunkOffset += FVector(0, 0, 1);
		z = ConvertVoxelToLocalZ(z);
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
			if (c->chunkIndex == (chunkIndex + chunkOffset)) {
				int listIndexValue = x + y * yMult + z * zMult;
				return c->voxels[listIndexValue].isSolid;
			}
		}
		return false; //Outside all read chunks.
	}
}

int AChunk::ConvertVoxelToLocalXY(int i)
{
	if (i == -1)
	{
		i = SizeXY - 1;
	}
	else if (i == SizeXY)
	{
		i = 0;
	}
	return i;
}

int AChunk::ConvertVoxelToLocalZ(int i)
{
	if (i == -1)
	{
		i = SizeZ - 1;
	}
	else if (i == SizeZ)
	{
		i = 0;
	}
	return i;
}
