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
	static enum RenderStage {LOAD, GENERATE, DRAW, NONE};

	// Sets default values for this actor's properties
	AWorldGenerator();

	UPROPERTY(EditAnywhere)
		AActor *player;
	UPROPERTY(EditAnywhere)
		int generationDistance = 3;
	UPROPERTY(EditAnywhere)
		int renderDistance = 6;
	UPROPERTY(EditAnywhere)
		UMaterial *material;

	UPROPERTY(EditAnywhere)
		UTexture2D* defaultAtlas_UTexture2D;

	TArray<class AChunk*> chunks;
	static const int chunkSize = AVoxel::voxelSize * AChunk::Dimensions;

private:
	int oldPlayerX;
	int oldPlayerY;
	int ring = 1; // Used to render around the player in steps
	int numZChunks = 3;
	bool hasMoved = false;
	RenderStage stage = RenderStage::LOAD;
	bool doesChunkExist(FVector index, AChunk::ChunkStatus& _status);

	FColor *defaultAtlas;

protected:
	UWorld* WRLD;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PostActorCreated() override;

	void PostLoad() override;

	virtual void EndPlay();

	bool loadChunk(FVector index);
	void makeStructures(FVector index);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
