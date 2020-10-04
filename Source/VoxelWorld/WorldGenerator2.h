// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "WorldSlice.h"

#include "WorldGenerator2.generated.h"

UCLASS()
class VOXELWORLD_API AWorldGenerator2 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldGenerator2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
	int renderDistance = 4;

	UPROPERTY(EditAnywhere)
		AActor* player;

private:
	UWorld* WRLD;

	TMap<FVector2D, class AWorldSlice*>WorldSlices;
	int ActiveRenderDistance = 1;

	// Player.
	FVector2D PlayerAtSlice;
	FVector2D OldPlayerAtSlice;

private:
	bool HasPlayerCrossedChunks();
	void DeleteUnnecessaryWorldSlices();

	void GenerateNewWorldSlices();
	bool DoesWorldSliceExist(FVector2D WSI);
	void GenerateWorldSlice(FVector2D WSI); // No check in this function if it already exist!

};