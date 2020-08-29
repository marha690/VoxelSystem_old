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
		int numZChunks = 3;
	UPROPERTY(EditAnywhere)
		UMaterial *material;

	UPROPERTY(EditAnywhere)
		UTexture2D* defaultAtlas_UTexture2D;

	TMap<FVector, class AChunk*>chunks;
	static const int chunkSize = AVoxel::voxelSize * AChunk::Dimensions;

private:
	int playerPosX;
	int playerPosY;
	int oldPlayerX;
	int oldPlayerY;
	int ring = 1; // Used to render around the player in steps
	bool hasMoved = false;
	RenderStage stage = RenderStage::LOAD;

	TArray<FColor> colorAtlas;

protected:
	UWorld* WRLD;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void readTexute();
	bool isReadyForNextStage();
	bool loadChunk(FVector index);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
