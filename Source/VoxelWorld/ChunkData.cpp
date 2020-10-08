
// Fill out your copyright notice in the Description page of Project Settings.


#include "ChunkData.h"
#include "WorldSlice.h"
#include "WorldGenerator2.h"

const FVector2D ChunkData::UV00 = FVector2D(0.f, 0.f);
const FVector2D ChunkData::UV10 = FVector2D(1.f, 0.f);
const FVector2D ChunkData::UV01 = FVector2D(0.f, 1.f);
const FVector2D ChunkData::UV11 = FVector2D(1.f, 1.f);

ChunkData::ChunkData()
{
	SliceAsOwner = nullptr;
	ZPos = -1;
}

ChunkData::ChunkData(AWorldSlice* Owner, int Z)
{
	SliceAsOwner = Owner;
	ZPos = Z;
}

ChunkData::~ChunkData()
{
}

void ChunkData::UpdateMeshData()
{
	for (int Y = 0; Y < WORLD_PROPERTIES::VoxelsPerChunkDimension; Y++)
		for (int X = 0; X < WORLD_PROPERTIES::VoxelsPerChunkDimension; X++)
			for (int Z = 0; Z < WORLD_PROPERTIES::VoxelsPerChunkDimension; Z++) {

				if (!IsSolid(bType[linearIndex(X, Y, Z)])) continue;

				FVector voxelPos = FVector(X, Y, Z);
				if (!HasSolidNeighbour(voxelPos.X - 1, voxelPos.Y, voxelPos.Z))
					CreateQuad(BACK, voxelPos);
				if (!HasSolidNeighbour(voxelPos.X + 1, voxelPos.Y, voxelPos.Z))
					CreateQuad(FRONT, voxelPos);
				if (!HasSolidNeighbour(voxelPos.X, voxelPos.Y - 1, voxelPos.Z))
					CreateQuad(LEFT, voxelPos);
				if (!HasSolidNeighbour(voxelPos.X, voxelPos.Y + 1, voxelPos.Z))
					CreateQuad(RIGHT, voxelPos);
				if (!HasSolidNeighbour(voxelPos.X, voxelPos.Y, voxelPos.Z + 1))
					CreateQuad(TOP, voxelPos);
				if (!HasSolidNeighbour(voxelPos.X, voxelPos.Y, voxelPos.Z - 1))
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

	p0 += FVector(0.f, 0.f, ZPos * WORLD_PROPERTIES::ChunkRealSize);
	p1 += FVector(0.f, 0.f, ZPos * WORLD_PROPERTIES::ChunkRealSize);
	p2 += FVector(0.f, 0.f, ZPos * WORLD_PROPERTIES::ChunkRealSize);
	p3 += FVector(0.f, 0.f, ZPos * WORLD_PROPERTIES::ChunkRealSize);
	p4 += FVector(0.f, 0.f, ZPos * WORLD_PROPERTIES::ChunkRealSize);
	p5 += FVector(0.f, 0.f, ZPos * WORLD_PROPERTIES::ChunkRealSize);
	p6 += FVector(0.f, 0.f, ZPos * WORLD_PROPERTIES::ChunkRealSize);
	p7 += FVector(0.f, 0.f, ZPos * WORLD_PROPERTIES::ChunkRealSize);

	// Vertex Color
	FColor color = GetColor(linearIndex(indexInChunk));

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
		VertexColors.Add(color);
		VertexColors.Add(color);
		VertexColors.Add(color);
		VertexColors.Add(color);
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
		VertexColors.Add(color);
		VertexColors.Add(color);
		VertexColors.Add(color);
		VertexColors.Add(color);
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
		VertexColors.Add(color);
		VertexColors.Add(color);
		VertexColors.Add(color);
		VertexColors.Add(color);
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
		VertexColors.Add(color);
		VertexColors.Add(color);
		VertexColors.Add(color);
		VertexColors.Add(color);
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
		VertexColors.Add(color);
		VertexColors.Add(color);
		VertexColors.Add(color);
		VertexColors.Add(color);
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
		VertexColors.Add(color);
		VertexColors.Add(color);
		VertexColors.Add(color);
		VertexColors.Add(color);
		break;
	}
}

void ChunkData::AddTriangle(int32 V1, int32 V2, int32 V3)
{
	Triangles.Add(V1);
	Triangles.Add(V2);
	Triangles.Add(V3);
}

bool ChunkData::HasSolidNeighbour(int x, int y, int z)
{
	BlockType v = getVoxel(x, y, z);
	return IsSolid(v);
}

void ChunkData::setVoxel(VOXEL::BlockType v, int x, int y, int z)
{
	int index = linearIndex(x, y, z);
	bType[index] = v;
	SliceAsOwner->isRendered = false;
}

BlockType& ChunkData::getVoxel(int x, int y, int z)
{
	int Dimensions = WORLD_PROPERTIES::VoxelsPerChunkDimension;

	//Move up and down in the chunks inside the world slice if needed.
	if (z < 0) {
		if (ZPos == 0)
			return blockade;
		z = ConvertVoxelToLocal(z);
		return SliceAsOwner->chunk[ZPos - 1].getVoxel(x, y, z);
	}
	else if (z > Dimensions - 1) {
		if (ZPos == WORLD_PROPERTIES::ChunksInHeight)
			return blockade;
		z = ConvertVoxelToLocal(z);
		return SliceAsOwner->chunk[ZPos + 1].getVoxel(x, y, z);
	}

	//Jump to other slices.
	if (x >= Dimensions || y >= Dimensions || x < 0 || y < 0) { // Inside other chunk.

		int newX = x;
		int newY = y;
		int newZ = z;

		FVector2D chunkOffset = FVector2D(0, 0);
		if (x < 0) {
			chunkOffset -= FVector2D(1, 0);
			x = ConvertVoxelToLocal(x);
		}
		else if (x > Dimensions - 1) {
			chunkOffset += FVector2D(1, 0);
			x = ConvertVoxelToLocal(x);
		}

		if (y < 0) {
			chunkOffset -= FVector2D(0, 1);
			y = ConvertVoxelToLocal(y);
		}
		else if (y > Dimensions - 1) {
			chunkOffset += FVector2D(0, 1);
			y = ConvertVoxelToLocal(y);
		}

		FVector2D SliceIndex = SliceAsOwner->SlicePositionIndex + chunkOffset;

		auto slice = SliceAsOwner->WorldAsOwner->GetWorldSlice(SliceIndex);

		if (slice) {
			int listIndex = linearIndex(x, y, z);
			return slice->chunk[ZPos].bType[listIndex];
		}

	}

	// Inside this chunk
	int index = linearIndex(x, y, z);
	return bType[index];
}

FColor ChunkData::GetColor(int LinearIndex)
{
	auto blockType = bType[LinearIndex];

	if (blockType == BlockType::GRASS)
		return FColor(96, 128, 56);
	if (blockType == BlockType::STONE)
		return FColor(153, 153, 153);
	if (blockType == BlockType::WOOD)
		return FColor(89, 60, 31);
	if (blockType == BlockType::LEAVES)
		return FColor(144, 238, 144);
	if (blockType == BlockType::BLUE)
		return FColor(0, 0, 150);

	return FColor();
}

int ChunkData::ConvertVoxelToLocal(int i)
{
	if (i <= -1) {
		i = WORLD_PROPERTIES::VoxelsPerChunkDimension + i;
	}
	else if (i >= WORLD_PROPERTIES::VoxelsPerChunkDimension) {
		i = i - WORLD_PROPERTIES::VoxelsPerChunkDimension;
	}
	return i;
}

int ChunkData::linearIndex(int x, int y, int z)
{
	return x + y * WORLD_PROPERTIES::VoxelsPerChunkDimension + z * WORLD_PROPERTIES::VoxelsPerChunkDimension * WORLD_PROPERTIES::VoxelsPerChunkDimension;
}

int ChunkData::linearIndex(FVector Pos)
{
	return linearIndex(Pos.X, Pos.Y, Pos.Z);
}

bool ChunkData::IsSolid(BlockType v)
{
	if (v == VOXEL::AIR) {
		return false;
	}
	else {
		return true;
	}
}
