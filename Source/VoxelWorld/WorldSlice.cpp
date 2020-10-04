// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldSlice.h"
#include "ChunkData.h"

// Sets default values
AWorldSlice::AWorldSlice()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CustomMesh = CreateDefaultSubobject<UProceduralMeshComponent>("CustomMesh");
	SetRootComponent(CustomMesh);
	CustomMesh->bUseAsyncCooking = true;

}

void AWorldSlice::Initialize(FVector2D SPI, AWorldGenerator2* WRLD)
{
	SlicePositionIndex = SPI;
	WorldAsOwner = WRLD;

	chunk.UpdateMeshData();
}

// Called when the game starts or when spawned
void AWorldSlice::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWorldSlice::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWorldSlice::RenderChunks()
{
	chunk.UpdateMeshData();
	CustomMesh->CreateMeshSection_LinearColor(0, chunk.Vertices, chunk.Triangles, chunk.Normals, chunk.UV0, chunk.VertexColors, TArray<FProcMeshTangent>(), true);
}

