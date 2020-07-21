// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldGenerator.h"
#include "Chunk.h"

// Sets default values
AWorldGenerator::AWorldGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWorldGenerator::BeginPlay()
{
	Super::BeginPlay();
	MakeTestChunkCube();
}

void AWorldGenerator::PostActorCreated()
{
	Super::PostActorCreated();

}

void AWorldGenerator::PostLoad()
{
	Super::PostLoad();

}

void AWorldGenerator::MakeTestChunkCube()
{
	FRotator rot = FRotator(0, 0, 0);

	UWorld* WRLD = GetWorld();
	if (GetWorld()) {
		for (size_t x = 0; x < chunksXY; x++) {
			for (size_t y = 0; y < chunksXY; y++) {
				FVector pos = FVector(x * chunkSize, y * chunkSize, 0);
				FVector index = FVector(x, y, 0);
				auto v = (AChunk*)GetWorld()->SpawnActor(AChunk::StaticClass(), &pos, &rot);
				v->Initialize(index, voxelsInChunkXY, voxelsInChunkZ, this);
				chunks.Add(v);
			}
		}
	}

	for (auto c : chunks) {
		c->RenderChunk();
	}

	//GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red, TEXT("Actor Spawning"));
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, pos.ToString());
}



// Called every frame
void AWorldGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

