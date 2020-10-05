// Fill out your copyright notice in the Description page of Project Settings.


#include "StructuresGenerator.h"
#include "WorldProperties.h"
#include <random>

StructuresGenerator::StructuresGenerator()
{
	RenderDistance = 16;
}

StructuresGenerator::StructuresGenerator(int RenderDist)
{
	RenderDistance = RenderDist;
}

StructuresGenerator::~StructuresGenerator()
{
}

void StructuresGenerator::GenerateArea(FVector2D PlayerPosition)
{
	int dist = RenderDistance + Level1 * 3; //?? not so many needed.

	//Make new.
	for (int x = -dist; x <= dist - 1; x++)
		for (int y = -dist; y <= dist - 1; y++) {
			FVector2D Index = FVector2D(x + PlayerPosition.X, y + PlayerPosition.Y);

			if (!DataInSlice.Contains(Index)) {
				StructureData newData = GenerateData(Index);
				DataInSlice.Add(Index, newData);
			}
		}
}

StructureData StructuresGenerator::GenerateData(FVector2D Index)
{
	StructureData data;

	StructureType t = GetStructureType(Index, Level1, LevelType::One);
	if (t == StructureType::None)
		t = GetStructureType(Index, Level2, LevelType::Two);
	if (t == StructureType::None)
		t = GetStructureType(Index, Level3, LevelType::Three);

	data.Type = t;
	return data;
}

StructureType StructuresGenerator::GetStructureType(FVector2D Index, int DistanceLevel, LevelType t)
{
	int maxRandomValue = 100;

	int lvX = 200 + abs((int)Index.X) - abs((int)Index.X) % DistanceLevel;
	int lvY = 200 + abs((int)Index.Y) - abs((int)Index.Y) % DistanceLevel;

	int rand = chash(lvX, lvY);

	int id = rand % maxRandomValue;

	switch (t)
	{
	case One:
	{

		break;
	}
	case Two:
	{
		if (id < 5)
			return StructureType::Village;

		break;
	}
	case Three:
	{
		if (id < 10)
			return StructureType::Tower;
		if (id < 20)
			return StructureType::Cabin;

		break;
	}
	default:
		break;
	}

	return StructureType::None;
}

void StructuresGenerator::RemoveGenerationsInSlices()
{
}

int StructuresGenerator::chash(int x, int y) {
	int h = seed + x * 374761393 + y * 668265263; //all constants are prime
	h = (h ^ (h >> 13)) * 1274126177;
	return h ^ (h >> 16);
}
