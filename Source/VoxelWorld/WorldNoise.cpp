// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldNoise.h"
#include "FastNoise.h"

#define Voxel(a) AVoxel(a, voxelLocalPos, voxelIndex)

//Main loop to generate the procedural noise for the game.
void WorldNoise::run(AVoxel &v, FVector position, FVector voxelLocalPos, int voxelIndex)
{
	int height = getGroundHeight(position);


	if (position.Z > height) {
		v = Voxel(AVoxel::AIR);
	}
	else {
		if (voxelIndex % 2 == 0)
			v = Voxel(AVoxel::STONE);
		else
			v = Voxel(AVoxel::GRASS);
	}

	if (position.Z == floor(height) && voxelLocalPos.X == 15 && voxelLocalPos.Y == 15)
		v = Voxel(AVoxel::TREESTART);
}

float WorldNoise::getGroundHeight(FVector position)
{
	FastNoise f = FastNoise(12);
	f.SetFractalOctaves(1);
	f.SetInterp(FastNoise::Interp::Linear);

	float adjust = 2.0f;
	auto val = abs(f.GetPerlin(position.X * adjust, position.Y * adjust)) * 40.0;
	val = floor(val);
	return val;
}

