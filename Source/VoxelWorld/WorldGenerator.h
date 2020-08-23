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
		int renderRadius = 10;
	UPROPERTY(EditAnywhere)
		int loadedAddition = 4; // adds generated chunks to rendered chunks.
	UPROPERTY(EditAnywhere)
		UMaterial *material;

	static const int voxelsInChunkXYZ = 16;
	static const int chunkSize = AVoxel::voxelSize * voxelsInChunkXYZ;
	TArray<class AChunk*> chunks;

	bool isStartup = true;
	bool isStartup2 = false;
protected:
	UWorld* WRLD;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PostActorCreated() override;

	void PostLoad() override;

	void GenerateChunks();

	void loadChunk(FVector index);
	void makeStructures(FVector index);
	void removeChunk(FVector index);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
