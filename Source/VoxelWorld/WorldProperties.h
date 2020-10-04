// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

namespace WORLD_PROPERTIES {
	static const int VoxelSize = 35;
	static const int VoxelsPerChunkDimension = 32;
	static const int VoxelsInChunk = VoxelsPerChunkDimension * VoxelsPerChunkDimension * VoxelsPerChunkDimension;
	static const int ChunkRealSize = VoxelsPerChunkDimension * VoxelSize;
	static const int ChunksInHeight = 2;
}
