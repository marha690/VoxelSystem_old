// Fill out your copyright notice in the Description page of Project Settings.


#include "Voxel.h"

AVoxel::AVoxel()
	: bType(BlockType::STONE), position(FVector(0.f, 0.f, 0.f))
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomMesh = CreateDefaultSubobject<UProceduralMeshComponent>("CustomMesh");
	SetRootComponent(CustomMesh);
	CustomMesh->bUseAsyncCooking = true;
}


AVoxel::AVoxel(BlockType b, FVector pos)
	: bType(b), position(pos)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomMesh = CreateDefaultSubobject<UProceduralMeshComponent>("CustomMesh");
	SetRootComponent(CustomMesh);
	CustomMesh->bUseAsyncCooking = true;
}

// Called when the game starts or when spawned
void AVoxel::BeginPlay()
{
	Super::BeginPlay();
}

void AVoxel::PostActorCreated()
{
	Super::PostActorCreated();
	GenerateCubeMesh();
}

void AVoxel::PostLoad()
{
	Super::PostLoad();
	GenerateCubeMesh();
}

void AVoxel::AddTriangle(int32 V1, int32 V2, int32 V3)
{
	Triangles.Add(V1);
	Triangles.Add(V2);
	Triangles.Add(V3);
}


void AVoxel::GenerateCubeMesh()
{
	//6 sides on cube, 4 verts each (corners)

	//These are relative locations to the placed Actor in the world
	Vertices.Add(FVector(-halfSize, -halfSize, -halfSize)); //lower left - 0
	Vertices.Add(FVector(-halfSize, -halfSize, halfSize)); //upper left - 1
	Vertices.Add(FVector(-halfSize, halfSize, -halfSize)); //lower right - 2 
	Vertices.Add(FVector(-halfSize, halfSize, halfSize)); //upper right - 3

	Vertices.Add(FVector(halfSize, -halfSize, -halfSize)); //lower front left - 4
	Vertices.Add(FVector(halfSize, -halfSize, halfSize)); //upper front left - 5

	Vertices.Add(FVector(halfSize, halfSize, halfSize)); //upper front right - 6
	Vertices.Add(FVector(halfSize, halfSize, -halfSize)); //lower front right - 7

	VertexColors.Add(FLinearColor(0.f, 0.f, 1.f));
	VertexColors.Add(FLinearColor(1.f, 0.f, 0.f));
	VertexColors.Add(FLinearColor(1.f, 0.f, 0.f));
	VertexColors.Add(FLinearColor(0.f, 1.f, 0.f));
	VertexColors.Add(FLinearColor(0.5f, 1.f, 0.5f));
	VertexColors.Add(FLinearColor(0.f, 1.f, 0.f));
	VertexColors.Add(FLinearColor(1.f, 1.f, 0.f));
	VertexColors.Add(FLinearColor(0.f, 1.f, 1.f));


	Draw();
}

void AVoxel::Draw()
{
	CreateQuad(Cubeside::BACK);
	CreateQuad(Cubeside::FRONT);
	CreateQuad(Cubeside::LEFT);
	CreateQuad(Cubeside::RIGHT);
	CreateQuad(Cubeside::TOP);
	CreateQuad(Cubeside::BOTTOM);
	CustomMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), VertexColors, TArray<FProcMeshTangent>(), true);
}

//Makes one side of the cube.
void AVoxel::CreateQuad(Cubeside side)
{
	switch (side)
	{
	case BOTTOM:
		//bottom face
		AddTriangle(2, 0, 4);
		AddTriangle(4, 7, 2);
		break;
	case TOP:
		//Top face
		AddTriangle(1, 3, 5);
		AddTriangle(6, 5, 3);
		break;
	case LEFT:
		//Left face of cube
		AddTriangle(0, 1, 4);
		AddTriangle(4, 1, 5);
		break;
	case RIGHT:
		//Right face of cube
		AddTriangle(7, 6, 3);
		AddTriangle(3, 2, 7);
		break;
	case FRONT:
		//Front face of cube
		AddTriangle(4, 5, 7);
		AddTriangle(7, 5, 6);
		break;
	case BACK:
		//Back face of cube
		AddTriangle(0, 2, 3);
		AddTriangle(3, 1, 0);
		break;
	}
}