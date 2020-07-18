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

	BuildChunk(5, 5);
}

void AChunk::BuildChunk(int32 sizeXY, int32 sizeZ)
{
	SizeXY = sizeXY;
	SizeZ = sizeZ;
	_maxNumbeOfVoxels = sizeXY * sizeXY * sizeZ;
	voxels = new AShapes[_maxNumbeOfVoxels];
	int c = 0;
	for (int Z = 0; Z < sizeZ; Z++) {
		for (int Y = 0; Y < sizeXY; Y++) {
			for (int X = 0; X < sizeXY; X++) {
				UWorld* WRLD = GetWorld();
				if (GetWorld()) {
					FVector index = FVector(X, Y, Z);
					//auto v = (AVoxel*)GetWorld()->SpawnActor(AVoxel::StaticClass(), &pos, &rot);

					if (Y < 2 && X < 2 && Z == 2) {
					voxels[c] = AShapes(AShapes::AIR, index, c);
					}
					else {
					voxels[c] = AShapes(AShapes::STONE, index, c);
					}

					voxels[c].GenerateCubeMesh(&Vertices, &VertexColors);
					c++;
				}
				//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, pos.ToString());
			}
		}
	}
	RenderChunk();
}

void AChunk::RenderChunk()
{
	for (size_t i = 0; i < _maxNumbeOfVoxels; i++)
	{
		if (voxels[i].isSolid) {
			auto& voxelPos = voxels[i].indexInChunk;
			if (!hasSolidNeighbour(voxelPos.X - 1, voxelPos.Y, voxelPos.Z))
				voxels[i].CreateQuad(AShapes::BACK, &Triangles);
			if (!hasSolidNeighbour(voxelPos.X + 1, voxelPos.Y, voxelPos.Z))
				voxels[i].CreateQuad(AShapes::FRONT, &Triangles);
			if (!hasSolidNeighbour(voxelPos.X, voxelPos.Y - 1, voxelPos.Z))
				voxels[i].CreateQuad(AShapes::LEFT, &Triangles);
			if (!hasSolidNeighbour(voxelPos.X, voxelPos.Y + 1, voxelPos.Z))
				voxels[i].CreateQuad(AShapes::RIGHT, &Triangles);
			if (!hasSolidNeighbour(voxelPos.X, voxelPos.Y, voxelPos.Z + 1))
				voxels[i].CreateQuad(AShapes::TOP, &Triangles);
			if (!hasSolidNeighbour(voxelPos.X, voxelPos.Y, voxelPos.Z - 1))
				voxels[i].CreateQuad(AShapes::BOTTOM, &Triangles);
		}
	}
	CustomMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), VertexColors, TArray<FProcMeshTangent>(), true);
}

bool AChunk::hasSolidNeighbour(int x, int y, int z)
{
	int yMult = SizeXY;
	int zMult = SizeXY * SizeXY;

	if (x < 0 || x > SizeXY - 1 || y < 0 || y > SizeXY - 1 || z < 0 || z > SizeZ - 1) {
		return false; //outside of the chunk.
	}

	int listIndexValue = x + y * yMult + z * zMult;
	auto& neighbouringVoxel = voxels[listIndexValue];
	return neighbouringVoxel.isSolid;
}
