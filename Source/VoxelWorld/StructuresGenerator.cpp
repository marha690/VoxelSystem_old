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

const StructureData& StructuresGenerator::GetData(FVector2D Index)
{
	if (!DataInSlice.Contains(Index)) {
		StructureData newData = GenerateData(Index);
		DataInSlice.Add(Index, newData);
	}

	return DataInSlice[Index];
}

void StructuresGenerator::DeleteGenerationsInSlices(FVector2D PlayerAtSlice)
{
	int distance = RenderDistance + Level1Size * 3;
	for (auto It = DataInSlice.CreateConstIterator(); It; ++It)
	{
		int x = It.Key().X;
		int y = It.Key().Y;

		if (abs(x - PlayerAtSlice.X) > distance ||
			abs(y - PlayerAtSlice.Y) > distance) {

			DataInSlice.Remove(It.Key()); //Does this remove values data from memory?
		}
	}
}

StructureData StructuresGenerator::GenerateData(FVector2D Index)
{
	StructureData data{StructureType::None};

	StructureType t = GetStructureType(Index, Level1Size, 1);
	if (t == StructureType::None)
		t = GetStructureType(Index, Level2Size, 2);
	if (t == StructureType::None)
		t = GetStructureType(Index, Level3Size, 3);
	data.Type = t;

	if (t != StructureType::None) {
		GenerateArea(data, Index);
	}

	return data;
}

StructureType StructuresGenerator::GetStructureType(FVector2D Index, int Distance, int Level)
{
	int maxRandomValue = 100;

	int lvX = 200 + abs((int)Index.X) - abs((int)Index.X) % Distance;
	int lvY = 200 + abs((int)Index.Y) - abs((int)Index.Y) % Distance;

	int rand = chash(lvX, lvY);

	int id = rand % maxRandomValue;

	if (Level == 1) {

	}
	else if (Level == 2) {
		if (id < 5)
			return StructureType::Village;

	}
	else if (Level == 3) {
		if (id < 10)
			return StructureType::Tower;
		if (id < 20)
			return StructureType::Cabin;
	}

	return StructureType::None;
}

void StructuresGenerator::GenerateArea(StructureData Data, FVector2D Index)
{
	//TODO: Algorithm for the different types of things inside the area.
}

int StructuresGenerator::chash(int x, int y) {
	int h = seed + x * 374761393 + y * 668265263; //all constants are prime
	h = (h ^ (h >> 13)) * 1274126177;
	return h ^ (h >> 16);
}
