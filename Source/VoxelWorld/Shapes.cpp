// Fill out your copyright notice in the Description page of Project Settings.


#include "Shapes.h"

AShapes::AShapes()
	: bType(BlockType::AIR), indexInChunk(FVector(0.f, 0.f, 0.f)), isSolid(false) {}

AShapes::AShapes(BlockType b, FVector pos, int index)
	: bType(b), indexInChunk(pos), listIndex(index)
{
	if (b == AIR) {
		isSolid = false;
	} else {
		isSolid = true;
	}
}

void AShapes::GenerateCubeMesh(TArray<FVector>* vertices, TArray<FLinearColor>* vertexColors)
{
	//Vertices
	vertices->Add(FVector(0 + indexInChunk.X * voxelSize, 0 + indexInChunk.Y * voxelSize, 0 + indexInChunk.Z * voxelSize)); //lower left - 0
	vertices->Add(FVector(0 + indexInChunk.X * voxelSize, 0 + indexInChunk.Y * voxelSize, voxelSize + indexInChunk.Z * voxelSize)); //upper left - 1
	vertices->Add(FVector(0 + indexInChunk.X * voxelSize, voxelSize + indexInChunk.Y * voxelSize, 0 + indexInChunk.Z * voxelSize)); //lower right - 2 
	vertices->Add(FVector(0 + indexInChunk.X * voxelSize, voxelSize + indexInChunk.Y * voxelSize, voxelSize + indexInChunk.Z * voxelSize)); //upper right - 3
	vertices->Add(FVector(voxelSize + indexInChunk.X * voxelSize, -0 + indexInChunk.Y * voxelSize, 0 + indexInChunk.Z * voxelSize)); //lower front left - 4
	vertices->Add(FVector(voxelSize + indexInChunk.X * voxelSize, 0 + indexInChunk.Y * voxelSize, voxelSize + indexInChunk.Z * voxelSize)); //upper front left - 5
	vertices->Add(FVector(voxelSize + indexInChunk.X * voxelSize, voxelSize + indexInChunk.Y * voxelSize, voxelSize + indexInChunk.Z * voxelSize)); //upper front right - 6
	vertices->Add(FVector(voxelSize + indexInChunk.X * voxelSize, voxelSize + indexInChunk.Y * voxelSize, 0 + indexInChunk.Z * voxelSize)); //lower front right - 7

	//Vertex color
	vertexColors->Add(FLinearColor(0.f, 0.f, 1.f));
	vertexColors->Add(FLinearColor(1.f, 0.f, 0.f));
	vertexColors->Add(FLinearColor(1.f, 0.f, 0.f));
	vertexColors->Add(FLinearColor(0.f, 1.f, 0.f));
	vertexColors->Add(FLinearColor(0.5f, 1.f, 0.5f));
	vertexColors->Add(FLinearColor(0.f, 1.f, 0.f));
	vertexColors->Add(FLinearColor(1.f, 1.f, 0.f));
	vertexColors->Add(FLinearColor(0.f, 1.f, 1.f));
}

void AShapes::CreateQuad(Cubeside side, TArray<int32> *triangles)
{
	switch (side)
	{
	case BOTTOM:
		//bottom face
		AddTriangle(2 + listIndex * 8, 0 + listIndex * 8, 4 + listIndex * 8, triangles);
		AddTriangle(4 + listIndex * 8, 7 + listIndex * 8, 2 + listIndex * 8, triangles);
		break;
	case TOP:
		//Top face
		AddTriangle(1 + listIndex * 8, 3 + listIndex * 8, 5 + listIndex * 8, triangles);
		AddTriangle(6 + listIndex * 8, 5 + listIndex * 8, 3 + listIndex * 8, triangles);
		break;
	case LEFT:
		//Left face of cube
		AddTriangle(0 + listIndex * 8, 1 + listIndex * 8, 4 + listIndex * 8, triangles);
		AddTriangle(4 + listIndex * 8, 1 + listIndex * 8, 5 + listIndex * 8, triangles);
		break;
	case RIGHT:
		//Right face of cube
		AddTriangle(7 + listIndex * 8, 6 + listIndex * 8, 3 + listIndex * 8, triangles);
		AddTriangle(3 + listIndex * 8, 2 + listIndex * 8, 7 + listIndex * 8, triangles);
		break;
	case FRONT:
		//Front face of cube
		AddTriangle(4 + listIndex * 8, 5 + listIndex * 8, 7 + listIndex * 8, triangles);
		AddTriangle(7 + listIndex * 8, 5 + listIndex * 8, 6 + listIndex * 8, triangles);
		break;
	case BACK:
		//Back face of cube
		AddTriangle(0 + listIndex * 8, 2 + listIndex * 8, 3 + listIndex * 8, triangles);
		AddTriangle(3 + listIndex * 8, 1 + listIndex * 8, 0 + listIndex * 8, triangles);
		break;
	}
}

void AShapes::AddTriangle(int32 V1, int32 V2, int32 V3, TArray<int32>* triangles)
{
	triangles->Add(V1);
	triangles->Add(V2);
	triangles->Add(V3);
}