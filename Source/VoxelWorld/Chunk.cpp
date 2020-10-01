// Fill out your copyright notice in the Description page of Project Settings.


#include "Chunk.h"
#include "Engine/World.h"
#include "WorldNoise.h"
#include "WorldGenerator.h"

const FVector2D AChunk::UV00 = FVector2D(0.f, 0.f);
const FVector2D AChunk::UV10 = FVector2D(1.f, 0.f);
const FVector2D AChunk::UV01 = FVector2D(0.f, 1.f);
const FVector2D AChunk::UV11 = FVector2D(1.f, 1.f);

AChunk::AChunk()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomMesh = CreateDefaultSubobject<UProceduralMeshComponent>("CustomMesh");
	SetRootComponent(CustomMesh);
	CustomMesh->bUseAsyncCooking = true;
}

void AChunk::Initialize(FVector cIndex, AWorldGenerator*_world, UMaterial *mat, TArray<FColor>* colors)
{
	UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(mat, this);
	CustomMesh->SetMaterial(0, DynMaterial);
	chunkIndex = cIndex;
	world = _world;
	status = ChunkStatus::LOAD;
	colorAtlas = colors;
	BuildChunk();
}

void AChunk::BuildChunk()
{
	(new FAutoDeleteAsyncTask<ChunkTask>(this))->StartBackgroundTask(); //Thread.
}

void AChunk::generateStructures() {

	//Trees
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
	status = ChunkStatus::DRAW;
}

bool AChunk::isSolid(VOXEL::BlockType b)
{
	if (b == VOXEL::AIR) {
		return false;
	}
	else {
		return true;
	}
}

void AChunk::RenderChunk()
{
	//Move to thread later if needed.
	for (int Y = 0; Y < Dimensions; Y++)
		for (int X = 0; X < Dimensions; X++)
			for (int Z = 0; Z < Dimensions; Z++) {
				if (!isSolid(bType[linearIndex(X,Y,Z)])) continue;
				FVector voxelPos = FVector(X, Y, Z);

				if (!hasSolidNeighbour(voxelPos.X - 1, voxelPos.Y, voxelPos.Z))
					CreateQuad(VOXEL::BACK, voxelPos);
				if (!hasSolidNeighbour(voxelPos.X + 1, voxelPos.Y, voxelPos.Z))
					CreateQuad(VOXEL::FRONT, voxelPos);
				if (!hasSolidNeighbour(voxelPos.X, voxelPos.Y - 1, voxelPos.Z))
					CreateQuad(VOXEL::LEFT, voxelPos);
				if (!hasSolidNeighbour(voxelPos.X, voxelPos.Y + 1, voxelPos.Z))
					CreateQuad(VOXEL::RIGHT, voxelPos);
				if (!hasSolidNeighbour(voxelPos.X, voxelPos.Y, voxelPos.Z + 1))
					CreateQuad(VOXEL::TOP, voxelPos);
				if (!hasSolidNeighbour(voxelPos.X, voxelPos.Y, voxelPos.Z - 1))
					CreateQuad(VOXEL::BOTTOM, voxelPos);
			}

	CustomMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, VertexColors, TArray<FProcMeshTangent>(), true); // Texture.

	status = ChunkStatus::DONE;
}

void AChunk::Destroyed()
{
}

void AChunk::buildTrees(int x, int y, int z)
{
	VOXEL::BlockType &t = getVoxel(x, y, z);

	if (t != VOXEL::TREESTART)
		return;

	getVoxel(x, y, z + 1) = VOXEL::WOOD;
	getVoxel(x, y, z + 2) = VOXEL::WOOD;

	//First floor.
	for (int i = -2; i <= 2; i++)
	{
		for (int j = -2; j <= 2; j++)
		{
			if ((i == j || i == -j) && (i == 2 || i == -2) )
				continue;

			getVoxel(x + i, y + j, z + 3) = VOXEL::WOOD;
		}
	}

	getVoxel(x, y, z + 4) = VOXEL::WOOD;
	getVoxel(x, y, z + 5) = VOXEL::WOOD;

	//second floor.
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			getVoxel(x + i, y + j, z + 5) = VOXEL::WOOD;
		}
	}

	getVoxel(x, y, z + 6) = VOXEL::LEAVES;

}

VOXEL::BlockType& AChunk::getVoxel(int x, int y, int z)
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

		if (world->chunks.Contains(newChunkIndex)) {
			verify(world->chunks[newChunkIndex]->status != AChunk::ChunkStatus::LOAD);
			int listIndex = linearIndex(x, y, z);
			return world->chunks[newChunkIndex]->bType[listIndex];
		}

	}

	// Inside chunk
	int index = linearIndex(x, y, z);
	return bType[index];
}
int AChunk::linearIndex(int x, int y, int z)
{
	return x + y * Dimensions + z * Dimensions * Dimensions;
}
FVector AChunk::getVoxelWorldPosition(FVector pos)
{
	return pos + chunkIndex * FVector(Dimensions, Dimensions, Dimensions);
}

bool AChunk::hasSolidNeighbour(int x, int y, int z)
{
	VOXEL::BlockType c = getVoxel(x, y, z);
	return isSolid(c);
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

//Rendering
void AChunk::CreateQuad(VOXEL::Cubeside side, FVector indexInChunk)
{
	int32 startIndex = Vertices.Num();

	FVector p0 = FVector(0 + indexInChunk.X * voxelSize, 0 + indexInChunk.Y * voxelSize, 0 + indexInChunk.Z * voxelSize); //lower left - 0
	FVector p1 = FVector(0 + indexInChunk.X * voxelSize, 0 + indexInChunk.Y * voxelSize, voxelSize + indexInChunk.Z * voxelSize); //upper left - 1
	FVector p2 = FVector(0 + indexInChunk.X * voxelSize, voxelSize + indexInChunk.Y * voxelSize, 0 + indexInChunk.Z * voxelSize); //lower right - 2 
	FVector p3 = FVector(0 + indexInChunk.X * voxelSize, voxelSize + indexInChunk.Y * voxelSize, voxelSize + indexInChunk.Z * voxelSize); //upper right - 3
	FVector p4 = FVector(voxelSize + indexInChunk.X * voxelSize, -0 + indexInChunk.Y * voxelSize, 0 + indexInChunk.Z * voxelSize); //lower front left - 4
	FVector p5 = FVector(voxelSize + indexInChunk.X * voxelSize, 0 + indexInChunk.Y * voxelSize, voxelSize + indexInChunk.Z * voxelSize); //upper front left - 5
	FVector p6 = FVector(voxelSize + indexInChunk.X * voxelSize, voxelSize + indexInChunk.Y * voxelSize, voxelSize + indexInChunk.Z * voxelSize); //upper front right - 6
	FVector p7 = FVector(voxelSize + indexInChunk.X * voxelSize, voxelSize + indexInChunk.Y * voxelSize, 0 + indexInChunk.Z * voxelSize); //lower front right - 7

	//Vertex Color
	FColor* c = colorAtlas->GetData();
	int index = (int)bType[linearIndex(indexInChunk.X, indexInChunk.Y, indexInChunk.Z)];
	FColor color = c[index];

	switch (side)
	{
	case VOXEL::BOTTOM:
		//Bottom face
		UV0.Add(UV00);
		UV0.Add(UV10);
		UV0.Add(UV01);
		UV0.Add(UV11);
		Vertices.Add(p2);
		Vertices.Add(p0);
		Vertices.Add(p7);
		Vertices.Add(p4);
		Normals.Add(FVector::DownVector);
		Normals.Add(FVector::DownVector);
		Normals.Add(FVector::DownVector);
		Normals.Add(FVector::DownVector);
		AddTriangle(startIndex, startIndex + 1, startIndex + 2);
		AddTriangle(startIndex + 3, startIndex + 2, startIndex + 1);
		VertexColors.Add(color);
		VertexColors.Add(color);
		VertexColors.Add(color);
		VertexColors.Add(color);
		break;
	case VOXEL::TOP:
		//Top face
		UV0.Add(UV00);
		UV0.Add(UV10);
		UV0.Add(UV01);
		UV0.Add(UV11);
		Vertices.Add(p1);
		Vertices.Add(p3);
		Vertices.Add(p5);
		Vertices.Add(p6);
		Normals.Add(FVector::UpVector);
		Normals.Add(FVector::UpVector);
		Normals.Add(FVector::UpVector);
		Normals.Add(FVector::UpVector);
		AddTriangle(startIndex, startIndex + 1, startIndex + 2);
		AddTriangle(startIndex + 3, startIndex + 2, startIndex + 1);
		VertexColors.Add(color);
		VertexColors.Add(color);
		VertexColors.Add(color);
		VertexColors.Add(color);
		break;
	case VOXEL::LEFT:
		//Left face of cube
		UV0.Add(UV00);
		UV0.Add(UV10);
		UV0.Add(UV01);
		UV0.Add(UV11);
		Vertices.Add(p0);
		Vertices.Add(p1);
		Vertices.Add(p4);
		Vertices.Add(p5);
		Normals.Add(FVector::LeftVector);
		Normals.Add(FVector::LeftVector);
		Normals.Add(FVector::LeftVector);
		Normals.Add(FVector::LeftVector);
		AddTriangle(startIndex, startIndex + 1, startIndex + 2);
		AddTriangle(startIndex + 3, startIndex + 2, startIndex + 1);
		VertexColors.Add(color);
		VertexColors.Add(color);
		VertexColors.Add(color);
		VertexColors.Add(color);
		break;
	case VOXEL::RIGHT:
		//Right face of cube
		UV0.Add(UV00);
		UV0.Add(UV10);
		UV0.Add(UV01);
		UV0.Add(UV11);
		Vertices.Add(p7);
		Vertices.Add(p6);
		Vertices.Add(p2);
		Vertices.Add(p3);
		Normals.Add(FVector::RightVector);
		Normals.Add(FVector::RightVector);
		Normals.Add(FVector::RightVector);
		Normals.Add(FVector::RightVector);
		AddTriangle(startIndex, startIndex + 1, startIndex + 2);
		AddTriangle(startIndex + 3, startIndex + 2, startIndex + 1);
		VertexColors.Add(color);
		VertexColors.Add(color);
		VertexColors.Add(color);
		VertexColors.Add(color);
		break;
	case VOXEL::FRONT:
		//Right face of cube
		UV0.Add(UV00);
		UV0.Add(UV10);
		UV0.Add(UV01);
		UV0.Add(UV11);
		Vertices.Add(p4);
		Vertices.Add(p5);
		Vertices.Add(p7);
		Vertices.Add(p6);
		Normals.Add(FVector::ForwardVector);
		Normals.Add(FVector::ForwardVector);
		Normals.Add(FVector::ForwardVector);
		Normals.Add(FVector::ForwardVector);
		AddTriangle(startIndex, startIndex + 1, startIndex + 2);
		AddTriangle(startIndex + 3, startIndex + 2, startIndex + 1);
		VertexColors.Add(color);
		VertexColors.Add(color);
		VertexColors.Add(color);
		VertexColors.Add(color);
		break;
	case VOXEL::BACK:
		//Back face of cube
		UV0.Add(UV00);
		UV0.Add(UV10);
		UV0.Add(UV01);
		UV0.Add(UV11);
		Vertices.Add(p3);
		Vertices.Add(p1);
		Vertices.Add(p2);
		Vertices.Add(p0);
		Normals.Add(FVector::BackwardVector);
		Normals.Add(FVector::BackwardVector);
		Normals.Add(FVector::BackwardVector);
		Normals.Add(FVector::BackwardVector);
		AddTriangle(startIndex, startIndex + 1, startIndex + 2);
		AddTriangle(startIndex + 3, startIndex + 2, startIndex + 1);
		VertexColors.Add(color);
		VertexColors.Add(color);
		VertexColors.Add(color);
		VertexColors.Add(color);
		break;
	}

}
void AChunk::AddTriangle(int32 V1, int32 V2, int32 V3)
{
	Triangles.Add(V1);
	Triangles.Add(V2);
	Triangles.Add(V3);
}

/******************
***	ChunkTask	***
******************/
ChunkTask::ChunkTask(AChunk* c)
	: chunk(c) {}

void ChunkTask::DoWork()
{
	for (int Y = 0; Y < chunk->Dimensions; Y++) {
		for (int X = 0; X < chunk->Dimensions; X++) {
			for (int Z = 0; Z < chunk->Dimensions; Z++) {
				auto globalPosition = chunk->getVoxelWorldPosition(FVector(X, Y, Z));

				int height = WorldNoise::getGroundHeight(globalPosition.X, globalPosition.Y);

				if (globalPosition.Z == height) {
					chunk->bType[chunk->linearIndex(X, Y, Z)] = VOXEL::GRASS;

					if (X == 10 && Y == 10) {
						chunk->bType[chunk->linearIndex(X, Y, Z)] = VOXEL::TREESTART;
					}
				}

				if (globalPosition.Z < height) {
					chunk->bType[chunk->linearIndex(X, Y, Z)] = VOXEL::STONE;
				}

			}
		}
	}


	chunk->status = chunk->ChunkStatus::GENERATE;
	return;
}

