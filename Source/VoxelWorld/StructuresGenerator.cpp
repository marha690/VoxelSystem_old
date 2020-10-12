// Fill out your copyright notice in the Description page of Project Settings.


#include "StructuresGenerator.h"
#include "WorldProperties.h"
#include <random>

/**** StructureGeneration ****/
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
	int xMod = abs((int)Index.X) % LevelSize;
	int yMod = abs((int)Index.Y) % LevelSize;

	int lvX = 0;
	int lvY = 0;
	if (Index.X >= 0)
		lvX = Index.X - xMod;
	else {
		Index.X += 1;
		xMod = abs((int)Index.X) % LevelSize;
		lvX = Index.X + xMod - 4;
	}

	if (Index.Y >= 0)
		lvY = Index.Y - yMod;
	else {
		Index.Y += 1;
		yMod = abs((int)Index.Y) % LevelSize;
		lvY = Index.Y + yMod - 4;
	}

	FVector2D indexSquish = FVector2D((int)lvX, (int)lvY);
	if (!DataInSlice.Contains(indexSquish)) {
		StructureData newData = GenerateData(indexSquish);
		DataInSlice.Add(indexSquish, newData);
	}

	return DataInSlice[indexSquish];
}

void StructuresGenerator::DeleteGenerationsInSlices(FVector2D PlayerAtSlice)
{
	int distance = RenderDistance + LevelSize * 3;
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
	StructureType t = GetStructureType(Index);
	StructureData data{ t , Structure(t,LevelSize)};
	return data;
}

StructureType StructuresGenerator::GetStructureType(FVector2D Index)
{
	int maxRandomValue = 100;

	int rand = chash((int)Index.X, (int)Index.Y);

	int id = rand % maxRandomValue;

	if (id < 10)
		return StructureType::Cabin;
	if (id < 20)
		return StructureType::Village;
	if (id < 30)
		return StructureType::Tower;

	return StructureType::None;
}

int StructuresGenerator::chash(int x, int y) {
	int h = seed + x * 374761393 + y * 668265263; //all constants are prime
	h = (h ^ (h >> 13)) * 1274126177;
	return h ^ (h >> 16);
}