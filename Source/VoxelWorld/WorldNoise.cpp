// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldNoise.h"
#include "Noise/cellular.h"
#include "Noise/noise1234.h"
#include "Noise/simplexnoise1234.h"

#define VOXEL(a) AVoxel(a, voxelLocalPos, voxelIndex, c)

//Main function to generate the procedural noise for the game.
void WorldNoise::run(AVoxel &v, FVector position, FVector voxelLocalPos, int voxelIndex, AChunk* c)
{
	int height = getGroundHeight(position);

	// Ground
	if (position.Z > height)
		v = VOXEL(AVoxel::AIR);
	else if (position.Z == height)
		v = VOXEL(AVoxel::GRASS);
	else
		v = VOXEL(AVoxel::STONE);

	// Trees
	if (position.Z == floor(height) && voxelLocalPos.X == 15 && voxelLocalPos.Y == 15)
		v = VOXEL(AVoxel::TREESTART);
}

float WorldNoise::getGroundHeight(FVector position)
{
	float scale= 0.005f;
	auto val = snoise2(position.X * scale, position.Y * scale);

	val = val * val;
	val = abs(val);


	float height = 30.0f;
	return val * height;
}