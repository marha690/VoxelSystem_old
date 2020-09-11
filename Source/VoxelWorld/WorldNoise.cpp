// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldNoise.h"
#include "Noise/cellular.h"
#include "Noise/noise1234.h"
#include "Noise/simplexnoise1234.h"

int WorldNoise::getGroundHeight(int x, int y)
{
	float scale = 0.005f;
	auto val = (snoise2(x * scale, y * scale) + 1) / 2;
	auto val2 = (snoise2(x * scale / 2, y * scale / 2) + 1) / 2;

	double a[3]{ x,y,0 };

	auto result = val*40 + val2*val2*20;
	result = abs(result);

	return result;
}
