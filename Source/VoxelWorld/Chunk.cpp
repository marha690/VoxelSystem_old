// Fill out your copyright notice in the Description page of Project Settings.


#include "Chunk.h"
#include "Engine/World.h"
#include "Voxel.h"
#include "Shapes.h"

AChunk::AChunk()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomMesh = CreateDefaultSubobject<UProceduralMeshComponent>("CustomMesh");
	SetRootComponent(CustomMesh);
	CustomMesh->bUseAsyncCooking = true;
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
	FVector pos = FVector(1, 1, 1);
	FVector pos1 = FVector(1, 1, 0);
	FRotator rot = FRotator(0, 0, 0);
	UWorld* WRLD = GetWorld();

	if (GetWorld()) {

		//auto s = GetWorld()->SpawnActor(ActorToSpawn, &pos, &rot);
		//auto v = (AVoxel*)GetWorld()->SpawnActor(AVoxel::StaticClass(), &pos, &rot);

	}

	//AShapes A = AShapes(AShapes::STONE, pos, 0);
	//A.GenerateCubeMesh(&Vertices, &VertexColors);
	//A.Draw(&Triangles);

	//CustomMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), VertexColors, TArray<FProcMeshTangent>(), true);

	BuildChunk(10, 10);
}


void AChunk::BuildChunk(int32 sizeXY, int32 sizeZ)
{
	_maxNumbeOfVoxels = sizeXY * sizeXY * sizeZ;
	voxels = new AShapes[_maxNumbeOfVoxels];
	int c = 0;
	for (int X = -sizeXY*0.5f; X < sizeXY*0.5f; X++)
	{
		for (int Y = -sizeXY * 0.5f; Y < sizeXY*0.5f; Y++)
		{
			for (int Z = -sizeZ * 0.5f; Z < sizeZ*0.5f; Z++)
			{
				FVector index = FVector(X, Y, Z);
				FRotator rot = FRotator(0, 0, 0);
				UWorld* WRLD = GetWorld();

				if (GetWorld()) {

					if (Z < 0) {

					//auto v = (AVoxel*)GetWorld()->SpawnActor(AVoxel::StaticClass(), &pos, &rot);
					voxels[c] = AShapes(AShapes::STONE, index, c);
					voxels[c].GenerateCubeMesh(&Vertices, &VertexColors);
					//voxels[c].Draw(&Triangles);
					_currentNumberOfVoxels++;
					c++;
					}

				}
				//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, pos.ToString());
			}
		}
	}

	RenderChunk();
}

void AChunk::RenderChunk()
{
	for (size_t i = 0; i < _currentNumberOfVoxels; i++)
	{
		voxels[i].Draw(&Triangles);
	}

	CustomMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), VertexColors, TArray<FProcMeshTangent>(), true);
}
