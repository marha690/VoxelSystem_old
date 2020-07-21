// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chunk.h"
#include "Voxel.h"
#include "WorldGenerator.generated.h"


UCLASS()
class VOXELWORLD_API AWorldGenerator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWorldGenerator();

	static const int chunksXY = 20;

	static const int voxelsInChunkXY = 6;
	static const int voxelsInChunkZ = 10;
	static const int chunkSize = AVoxel::voxelSize * voxelsInChunkXY;
	TArray<class AChunk*> chunks;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PostActorCreated() override;

	void PostLoad() override;

	void MakeTestChunkCube();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
