// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldNoise.h"
#include "Noise/cellular.h"
#include "Noise/noise1234.h"
#include "Noise/simplexnoise1234.h"
#include <algorithm>

int WorldNoise::getGroundHeight(int x, int y)
{
	float scale = 0.002f;

	auto height1 = (snoise2(x * scale, y * scale) + 1) / 2;
	auto height2 = (snoise2(x * scale * 2, y * scale * 2) + 1) / 2;
	auto height3 = (snoise2(x * scale * 4, y * scale * 4) + 1) / 2;
	auto result = 1 * height1 + 0.5 * height2 + 0.25 * height3;
	result = pow(result, 2.0);

	result = result * 60;



	////Test for worley noise.
	//size_t maxOrder = 4;
	//double at[3] = { x * scale, y * scale, 0 };
	//double* F = new double[maxOrder];
	//double(*delta)[3] = new double[maxOrder][3];
	//unsigned long* ID = new unsigned long[maxOrder];
	//Worley(at, maxOrder, F, delta, ID);

	//auto min0 = abs(delta[0][0] + delta[0][1] + delta[0][2]);
	//auto min1 = abs(delta[1][0] + delta[1][1] + delta[1][2]);
	//auto min2 = abs(delta[2][0] + delta[2][1] + delta[2][2]);
	//auto min3 = abs(delta[3][0] + delta[3][1] + delta[3][2]);

	//result = std::min(std::min(std::min(min0, min1),min2), min3);

	//result *= 34;

	//delete[] F;
	//delete[] delta;
	//delete[] ID;
	

	return result;
}
