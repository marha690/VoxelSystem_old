// Fill out your copyright notice in the Description page of Project Settings.


#include "ChunkData.h"


const FVector2D ChunkData::UV00 = FVector2D(0.f, 0.f);
const FVector2D ChunkData::UV10 = FVector2D(1.f, 0.f);
const FVector2D ChunkData::UV01 = FVector2D(0.f, 1.f);
const FVector2D ChunkData::UV11 = FVector2D(1.f, 1.f);

ChunkData::ChunkData(AWorldSlice* Owner)
{
	SliceAsOwner = Owner;
}

ChunkData::~ChunkData()
{
}

void ChunkData::UpdateMeshData()
{
	for (int Y = 0; Y < WORLD_PROPERTIES::VoxelsPerChunkDimension; Y++)
		for (int X = 0; X < WORLD_PROPERTIES::VoxelsPerChunkDimension; X++)
			for (int Z = 0; Z < WORLD_PROPERTIES::VoxelsPerChunkDimension; Z++) {

				//TODO: AIR
				//if (!isSolid(bType[linearIndex(X, Y, Z)])) continue;
				FVector voxelPos = FVector(X, Y, Z);

				//if (!hasSolidNeighbour(voxelPos.X - 1, voxelPos.Y, voxelPos.Z))
					CreateQuad(BACK, voxelPos);
				//if (!hasSolidNeighbour(voxelPos.X + 1, voxelPos.Y, voxelPos.Z))
					CreateQuad(FRONT, voxelPos);
				//if (!hasSolidNeighbour(voxelPos.X, voxelPos.Y - 1, voxelPos.Z))
					CreateQuad(LEFT, voxelPos);
				//if (!hasSolidNeighbour(voxelPos.X, voxelPos.Y + 1, voxelPos.Z))
					CreateQuad(RIGHT, voxelPos);
				//if (!hasSolidNeighbour(voxelPos.X, voxelPos.Y, voxelPos.Z + 1))
					CreateQuad(TOP, voxelPos);
				//if (!hasSolidNeighbour(voxelPos.X, voxelPos.Y, voxelPos.Z - 1))
					CreateQuad(BOTTOM, voxelPos);
			}
}

void ChunkData::CreateQuad(Cubeside side, FVector indexInChunk)
{
	int32 startIndex = Vertices.Num();

	FVector p0 = FVector(0 + indexInChunk.X * WORLD_PROPERTIES::VoxelSize, 0 + indexInChunk.Y * WORLD_PROPERTIES::VoxelSize, 0 + indexInChunk.Z * WORLD_PROPERTIES::VoxelSize); //lower left - 0
	FVector p1 = FVector(0 + indexInChunk.X * WORLD_PROPERTIES::VoxelSize, 0 + indexInChunk.Y * WORLD_PROPERTIES::VoxelSize, WORLD_PROPERTIES::VoxelSize + indexInChunk.Z * WORLD_PROPERTIES::VoxelSize); //upper left - 1
	FVector p2 = FVector(0 + indexInChunk.X * WORLD_PROPERTIES::VoxelSize, WORLD_PROPERTIES::VoxelSize + indexInChunk.Y * WORLD_PROPERTIES::VoxelSize, 0 + indexInChunk.Z * WORLD_PROPERTIES::VoxelSize); //lower right - 2 
	FVector p3 = FVector(0 + indexInChunk.X * WORLD_PROPERTIES::VoxelSize, WORLD_PROPERTIES::VoxelSize + indexInChunk.Y * WORLD_PROPERTIES::VoxelSize, WORLD_PROPERTIES::VoxelSize + indexInChunk.Z * WORLD_PROPERTIES::VoxelSize); //upper right - 3
	FVector p4 = FVector(WORLD_PROPERTIES::VoxelSize + indexInChunk.X * WORLD_PROPERTIES::VoxelSize, -0 + indexInChunk.Y * WORLD_PROPERTIES::VoxelSize, 0 + indexInChunk.Z * WORLD_PROPERTIES::VoxelSize); //lower front left - 4
	FVector p5 = FVector(WORLD_PROPERTIES::VoxelSize + indexInChunk.X * WORLD_PROPERTIES::VoxelSize, 0 + indexInChunk.Y * WORLD_PROPERTIES::VoxelSize, WORLD_PROPERTIES::VoxelSize + indexInChunk.Z * WORLD_PROPERTIES::VoxelSize); //upper front left - 5
	FVector p6 = FVector(WORLD_PROPERTIES::VoxelSize + indexInChunk.X * WORLD_PROPERTIES::VoxelSize, WORLD_PROPERTIES::VoxelSize + indexInChunk.Y * WORLD_PROPERTIES::VoxelSize, WORLD_PROPERTIES::VoxelSize + indexInChunk.Z * WORLD_PROPERTIES::VoxelSize); //upper front right - 6
	FVector p7 = FVector(WORLD_PROPERTIES::VoxelSize + indexInChunk.X * WORLD_PROPERTIES::VoxelSize, WORLD_PROPERTIES::VoxelSize + indexInChunk.Y * WORLD_PROPERTIES::VoxelSize, 0 + indexInChunk.Z * WORLD_PROPERTIES::VoxelSize); //lower front right - 7

	//Vertex Color
	//FColor* c = colorAtlas->GetData();
	//int index = (int)bType[linearIndex(indexInChunk.X, indexInChunk.Y, indexInChunk.Z)];
	//FColor color = c[index];

	switch (side)
	{
	case BOTTOM:
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
		//VertexColors.Add(color);
		//VertexColors.Add(color);
		//VertexColors.Add(color);
		//VertexColors.Add(color);
		break;
	case TOP:
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
		//VertexColors.Add(color);
		//VertexColors.Add(color);
		//VertexColors.Add(color);
		//VertexColors.Add(color);
		break;
	case LEFT:
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
		//VertexColors.Add(color);
		//VertexColors.Add(color);
		//VertexColors.Add(color);
		//VertexColors.Add(color);
		break;
	case RIGHT:
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
		//VertexColors.Add(color);
		//VertexColors.Add(color);
		//VertexColors.Add(color);
		//VertexColors.Add(color);
		break;
	case FRONT:
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
		//VertexColors.Add(color);
		//VertexColors.Add(color);
		//VertexColors.Add(color);
		//VertexColors.Add(color);
		break;
	case BACK:
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
		//VertexColors.Add(color);
		//VertexColors.Add(color);
		//VertexColors.Add(color);
		//VertexColors.Add(color);
		break;
	}
}

void ChunkData::AddTriangle(int32 V1, int32 V2, int32 V3)
{
	Triangles.Add(V1);
	Triangles.Add(V2);
	Triangles.Add(V3);
}
