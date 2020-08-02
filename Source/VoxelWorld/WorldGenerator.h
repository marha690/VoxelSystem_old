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

	UPROPERTY(EditAnywhere)
		AActor *player;
	UPROPERTY(EditAnywhere)
		int renderRadius = 15;

	static const int voxelsInChunkXYZ = 16;
	static const int chunkSize = AVoxel::voxelSize * voxelsInChunkXYZ;
	TArray<class AChunk*> chunks;

	bool isBuilding = false;
protected:
	UWorld* WRLD;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PostActorCreated() override;

	void PostLoad() override;

	void GenerateChunks();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
