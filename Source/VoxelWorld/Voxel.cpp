// Fill out your copyright notice in the Description page of Project Settings.


#include "Voxel.h"

const FVector2D AVoxel::UV00 = FVector2D(0.f, 0.f);
const FVector2D AVoxel::UV10 = FVector2D(1.f, 0.f);
const FVector2D AVoxel::UV01 = FVector2D(0.f, 1.f);
const FVector2D AVoxel::UV11 = FVector2D(1.f, 1.f);

AVoxel::AVoxel()
	: bType(BlockType::AIR), indexInChunk(FVector(-1.f, -1.f, -1.f)), solid(false) {}

AVoxel::AVoxel(BlockType b, FVector pos, int index)
	: bType(b), indexInChunk(pos), listIndex(index)
{
	setVoxelType(b);

	p0 = FVector(0 + indexInChunk.X * voxelSize, 0 + indexInChunk.Y * voxelSize, 0 + indexInChunk.Z * voxelSize); //lower left - 0
	p1 = FVector(0 + indexInChunk.X * voxelSize, 0 + indexInChunk.Y * voxelSize, voxelSize + indexInChunk.Z * voxelSize); //upper left - 1
	p2 = FVector(0 + indexInChunk.X * voxelSize, voxelSize + indexInChunk.Y * voxelSize, 0 + indexInChunk.Z * voxelSize); //lower right - 2 
	p3 = FVector(0 + indexInChunk.X * voxelSize, voxelSize + indexInChunk.Y * voxelSize, voxelSize + indexInChunk.Z * voxelSize); //upper right - 3
	p4 = FVector(voxelSize + indexInChunk.X * voxelSize, -0 + indexInChunk.Y * voxelSize, 0 + indexInChunk.Z * voxelSize); //lower front left - 4
	p5 = FVector(voxelSize + indexInChunk.X * voxelSize, 0 + indexInChunk.Y * voxelSize, voxelSize + indexInChunk.Z * voxelSize); //upper front left - 5
	p6 = FVector(voxelSize + indexInChunk.X * voxelSize, voxelSize + indexInChunk.Y * voxelSize, voxelSize + indexInChunk.Z * voxelSize); //upper front right - 6
	p7 = FVector(voxelSize + indexInChunk.X * voxelSize, voxelSize + indexInChunk.Y * voxelSize, 0 + indexInChunk.Z * voxelSize); //lower front right - 7
}

void AVoxel::setVoxelType(BlockType type)
{
	bType = type;
	if (type == AIR) {
		solid = false;
	}
	else {
		solid = true;
	}
}

void AVoxel::CreateQuad(Cubeside side, TArray<FVector>* vertices, TArray<int32> *triangles, TArray<FVector2D> *uv, TArray<FVector>* normals, TArray<FLinearColor>* vertexColors)
{
	int32 startIndex = vertices->Num();

	switch (side)
	{
	case BOTTOM:
		//Bottom face
		uv->Add(UV00);
		uv->Add(UV10);
		uv->Add(UV01);
		uv->Add(UV11);
		vertices->Add(p2);
		vertices->Add(p0);
		vertices->Add(p7);
		vertices->Add(p4);
		normals->Add(FVector::DownVector);
		normals->Add(FVector::DownVector);
		normals->Add(FVector::DownVector);
		normals->Add(FVector::DownVector);
		AddTriangle(startIndex, startIndex + 1, startIndex + 2, triangles);
		AddTriangle(startIndex + 3, startIndex + 2, startIndex + 1, triangles);
		vertexColors->Add(getVertexColor());
		vertexColors->Add(getVertexColor());
		vertexColors->Add(getVertexColor());
		vertexColors->Add(getVertexColor());
		break;
	case TOP:
		//Top face
		uv->Add(UV00);
		uv->Add(UV10);
		uv->Add(UV01);
		uv->Add(UV11);
		vertices->Add(p1);
		vertices->Add(p3);
		vertices->Add(p5);
		vertices->Add(p6);
		normals->Add(FVector::UpVector);
		normals->Add(FVector::UpVector);
		normals->Add(FVector::UpVector);
		normals->Add(FVector::UpVector);
		AddTriangle(startIndex, startIndex + 1, startIndex + 2, triangles);
		AddTriangle(startIndex + 3, startIndex + 2, startIndex + 1, triangles);
		vertexColors->Add(getVertexColor());
		vertexColors->Add(getVertexColor());
		vertexColors->Add(getVertexColor());
		vertexColors->Add(getVertexColor());
		break;
	case LEFT:
		//Left face of cube
		uv->Add(UV00);
		uv->Add(UV10);
		uv->Add(UV01);
		uv->Add(UV11);
		vertices->Add(p0);
		vertices->Add(p1);
		vertices->Add(p4);
		vertices->Add(p5);
		normals->Add(FVector::LeftVector);
		normals->Add(FVector::LeftVector);
		normals->Add(FVector::LeftVector);
		normals->Add(FVector::LeftVector);
		AddTriangle(startIndex, startIndex + 1, startIndex + 2, triangles);
		AddTriangle(startIndex + 3, startIndex + 2, startIndex + 1, triangles);
		vertexColors->Add(getVertexColor());
		vertexColors->Add(getVertexColor());
		vertexColors->Add(getVertexColor());
		vertexColors->Add(getVertexColor());
		break;
	case RIGHT:
		//Right face of cube
		uv->Add(UV00);
		uv->Add(UV10);
		uv->Add(UV01);
		uv->Add(UV11);
		vertices->Add(p7);
		vertices->Add(p6);
		vertices->Add(p2);
		vertices->Add(p3);
		normals->Add(FVector::RightVector);
		normals->Add(FVector::RightVector);
		normals->Add(FVector::RightVector);
		normals->Add(FVector::RightVector);
		AddTriangle(startIndex, startIndex + 1, startIndex + 2, triangles);
		AddTriangle(startIndex + 3, startIndex + 2, startIndex + 1, triangles);
		vertexColors->Add(getVertexColor());
		vertexColors->Add(getVertexColor());
		vertexColors->Add(getVertexColor());
		vertexColors->Add(getVertexColor());
		break;
	case FRONT:
		//Right face of cube
		uv->Add(UV00);
		uv->Add(UV10);
		uv->Add(UV01);
		uv->Add(UV11);
		vertices->Add(p4);
		vertices->Add(p5);
		vertices->Add(p7);
		vertices->Add(p6);
		normals->Add(FVector::ForwardVector);
		normals->Add(FVector::ForwardVector);
		normals->Add(FVector::ForwardVector);
		normals->Add(FVector::ForwardVector);
		AddTriangle(startIndex, startIndex + 1, startIndex + 2, triangles);
		AddTriangle(startIndex + 3, startIndex + 2, startIndex + 1, triangles);
		vertexColors->Add(getVertexColor());
		vertexColors->Add(getVertexColor());
		vertexColors->Add(getVertexColor());
		vertexColors->Add(getVertexColor());
		break;
	case BACK:
		//Back face of cube
		uv->Add(UV00);
		uv->Add(UV10);
		uv->Add(UV01);
		uv->Add(UV11);
		vertices->Add(p3);
		vertices->Add(p1);
		vertices->Add(p2);
		vertices->Add(p0);
		normals->Add(FVector::BackwardVector);
		normals->Add(FVector::BackwardVector);
		normals->Add(FVector::BackwardVector);
		normals->Add(FVector::BackwardVector);
		AddTriangle(startIndex, startIndex + 1, startIndex + 2, triangles);
		AddTriangle(startIndex + 3, startIndex + 2, startIndex + 1, triangles);
		vertexColors->Add(getVertexColor());
		vertexColors->Add(getVertexColor());
		vertexColors->Add(getVertexColor());
		vertexColors->Add(getVertexColor());
		break;
	}

}

FLinearColor AVoxel::getVertexColor()
{
	switch (bType)
	{
	case AVoxel::STONE:
		return FLinearColor::Gray;
		break;
	case AVoxel::GRASS:
		return FLinearColor(0,123,123); //cyan
		break;
	case AVoxel::LEAVES:
		return FLinearColor(FColor(0, 105, 30));
		break;
	case AVoxel::TREESTART:
	case AVoxel::WOOD:
		return FLinearColor(FColor(210,105,30)); //brown
		break;
	default:
		return FLinearColor::Black;
		break;
	}
}

void AVoxel::AddTriangle(int32 V1, int32 V2, int32 V3, TArray<int32>* triangles)
{
	triangles->Add(V1);
	triangles->Add(V2);
	triangles->Add(V3);
}