// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

namespace WORLDSLICETRACKER {
	enum SliceState
	{
		Exist,
		Terrain,
		Structures
	};
};
/**
 * 
 */
class VOXELWORLD_API WorldSliceTracker
{
public:
	WorldSliceTracker();
	~WorldSliceTracker();


};
