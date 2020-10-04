// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ChunkData.h"

#include "ProceduralMeshComponent.h"
#include "WorldSlice.generated.h"

class AWorldGenerator2;

UCLASS()
class VOXELWORLD_API AWorldSlice : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldSlice();
	void Initialize(FVector2D SPI, AWorldGenerator2* WRLD);


protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void RenderChunks();

private:
	FVector2D SlicePositionIndex; // Chunk based coordinate system.
	AWorldGenerator2* WorldAsOwner; // Which worldGenerator has generated this slice.

	ChunkData chunk{this};
	UProceduralMeshComponent* CustomMesh;

};
