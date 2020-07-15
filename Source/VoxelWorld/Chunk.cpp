// Fill out your copyright notice in the Description page of Project Settings.


#include "Chunk.h"

#include "Engine/World.h"

#include "Voxel.h"

AChunk::AChunk()
{
}


void AChunk::PostActorCreated() 
{
	Super::PostActorCreated();

	//FVector pos = FVector(1, 1, 300);
	//FVector pos1 = FVector(1, 1, 0);
	//FRotator rot = FRotator(0, 0, 0);
	//UWorld* WRLD = GetWorld();

	//if (GetWorld()) {
	//	GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Red, TEXT("Actor Spawning"));

	//	auto s = GetWorld()->SpawnActor(ActorToSpawn, &pos, &rot);

	//	auto v = (AVoxel *) GetWorld()->SpawnActor(AVoxel::StaticClass(), &pos1, &rot);
	//}

}


void AChunk::PostLoad()
{
	Super::PostLoad();

	//FVector pos = FVector(1, 1, 300);
	//FVector pos1 = FVector(1, 1, 0);
	//FRotator rot = FRotator(0, 0, 0);
	//UWorld* WRLD = GetWorld();

	//if (GetWorld()) {

	//	auto v = (AVoxel*)GetWorld()->SpawnActor(AVoxel::StaticClass(), &pos1, &rot);
	//}

}

void AChunk::BeginPlay()
{
	Super::BeginPlay();
	//BuildChunk(1, 1);
	FVector pos = FVector(1, 1, 300);
	FVector pos1 = FVector(1, 1, 0);
	FRotator rot = FRotator(0, 0, 0);
	UWorld* WRLD = GetWorld();

	GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Red, TEXT("Actor Spawning"));
	if (GetWorld()) {

		//auto s = GetWorld()->SpawnActor(ActorToSpawn, &pos, &rot);
		auto v = (AVoxel*)GetWorld()->SpawnActor(AVoxel::StaticClass(), &pos, &rot);
	}

	BuildChunk(10, 10);
}


void AChunk::BuildChunk(int32 sizeXY, int32 sizeZ)
{
	for (int X = 0; X < sizeXY; X++)
	{
		for (int Y = 0; Y < sizeXY; Y++)
		{
			for (int Z = 0; Z < sizeZ; Z++)
			{
				FVector pos = FVector(X * 200, Y * 200, Z * 200);
				FRotator rot = FRotator(0, 0, 0);
				UWorld* WRLD = GetWorld();

				if (GetWorld()) {

					auto v = (AVoxel*)GetWorld()->SpawnActor(AVoxel::StaticClass(), &pos, &rot);
				}
			}
		}
	}
}

