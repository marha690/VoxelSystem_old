// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chunk.h"
#include "Shapes.h"
#include "testActor.generated.h"


UCLASS()
class VOXELWORLD_API AtestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AtestActor();

	static const int chunksXY = 2;

	static const int voxelsInChunkXY = 6;
	static const int voxelsInChunkZ = 6;
	static const int chunkSize = AShapes::voxelSize * voxelsInChunkXY;
	TArray<class AChunk*> chunks;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MakeTestChunkCube();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
