// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chunk.generated.h"



/**
 * 
 */
UCLASS()
class VOXELWORLD_API AChunk : public AActor
{
	GENERATED_BODY()

public:

	//UPROPERTY(EditAnywhere)
		//TSubclassOf<AActor> ActorToSpawn;

	AChunk();

	void BuildChunk(int32 sizeXY, int32 sizeZ);

	// Called actor is dragged/spawned in the world.
	virtual void PostActorCreated() override;

	//Level opened.
protected:
	virtual void PostLoad() override;


	virtual void BeginPlay() override;

public:
	// Called every frame

		

};
