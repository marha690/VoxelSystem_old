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

	auto height1 = (snoise2(x * scale, y * scale) + 1) / 2;
	auto height2 = (snoise2(x * scale * 2, y * scale * 2) + 1) / 2;
	auto height3 = (snoise2(x * scale * 4, y * scale * 4) + 1) / 2;
	auto result = 1 * height1 + 0.5 * height2 + 0.25 * height3;
	result = pow(result, 2.0);

	result = result * 30;

	return result;
}
