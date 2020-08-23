// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldGenerator.h"
#include "Chunk.h"

// Sets default values
AWorldGenerator::AWorldGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWorldGenerator::BeginPlay()
{
	Super::BeginPlay();
	WRLD = GetWorld();
	verify(WRLD != nullptr);
}

void AWorldGenerator::PostActorCreated()
{
	Super::PostActorCreated();
}

void AWorldGenerator::PostLoad()
{
	Super::PostLoad();
}

void AWorldGenerator::GenerateChunks()
{
	int playerPosX = (int)floor(player->GetTransform().GetLocation().X / chunkSize); //the chunk index whiuch
	int playerPosY = (int)floor(player->GetTransform().GetLocation().Y / chunkSize);

	// Load chunks.
	int radius = renderRadius + loadedAddition;
	for (int x = -radius; x <= radius; x++) {
		for (int y = -radius; y <= radius; y++) {
			FVector index = FVector(x + playerPosX, y + playerPosY, 0);
			loadChunk(index);
		}
	}

	// Generate objects.
	for (auto c : chunks) {
		int renderRectangle = renderRadius + (loadedAddition / 2);
		if ((abs(c->chunkIndex.X - playerPosX) == renderRectangle && abs(c->chunkIndex.Y - playerPosY) <= renderRectangle) ||
			(abs(c->chunkIndex.Y - playerPosY) == renderRectangle) && abs(c->chunkIndex.X - playerPosX) <= renderRectangle) {
			if (c->status == AChunk::ChunkStatus::STRUCTURES) {
				c->generateStructures();
			}
		}
	}

	// Render chunks
	for (auto c : chunks) {
		if (abs(c->chunkIndex.Y - playerPosY) <= renderRadius && abs(c->chunkIndex.X - playerPosX) <= renderRadius) {
			if (c->status == AChunk::ChunkStatus::DRAW) {
				c->RenderChunk();
			}
		}
	}

	// Delete chunks
	for (auto c : chunks) {
		//if (abs(c->chunkIndex.Y - playerPosY) <= renderRadius && abs(c->chunkIndex.X - playerPosX) <= renderRadius) {
		if (abs(c->chunkIndex.Y - playerPosY) > radius || abs(c->chunkIndex.X - playerPosX) > radius) {
				chunks.Remove(c);
				c->Destroy();
				break;
		}
	}
}

void AWorldGenerator::loadChunk(FVector index)
{
	// Check if the chunk already is loaded.
	bool newChunk = true;
	for (auto c : chunks) {
		auto chunkIndex = c->chunkIndex;
		if (index == chunkIndex) {
			newChunk = false;
		}
	}

	// Create new chunk if it does not exist.
	if (newChunk) {
		FRotator rot = FRotator(0, 0, 0);
		auto pos = (index * chunkSize);
		auto v = (AChunk*)GetWorld()->SpawnActor(AChunk::StaticClass(), &pos, &rot);
		v->Initialize(index, voxelsInChunkXYZ, this, material);
		v->SetFolderPath("/Chunks");
		chunks.Add(v);
	}
}

void AWorldGenerator::makeStructures(FVector index)
{
	for (auto c : chunks) {
		auto chunkIndex = c->chunkIndex;
		if (index == chunkIndex) {
			c->generateStructures();
		}
	}
}

// Called every frame
void AWorldGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(isStartup)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("starts..."));

		// Load chunks.
		int playerPosX = (int)floor(player->GetTransform().GetLocation().X / chunkSize);
		int playerPosY = (int)floor(player->GetTransform().GetLocation().Y / chunkSize);

		int radius = renderRadius + loadedAddition;
		for (int x = -radius; x <= radius; x++) {
			for (int y = -radius; y <= radius; y++) {
				FVector index = FVector(x + playerPosX, y + playerPosY, 0);
				loadChunk(index);
			}
		}

		for (auto c : chunks) {
			isStartup = false;
			isStartup2 = true;
			if (c->status != AChunk::ChunkStatus::STRUCTURES) {
				isStartup = true;
				isStartup2 = false;
				break;
			}
		}
	}
	else if (isStartup2)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("starts2..."));

		// generate chunks
		for (auto c : chunks) {
			if (c->status == AChunk::ChunkStatus::STRUCTURES) {
				c->generateStructures();
				c->status = AChunk::ChunkStatus::DRAW;
			}
		}

		for (auto c : chunks) {
			isStartup2 = false;
			if (c->status != AChunk::ChunkStatus::DRAW) {
				isStartup = true;
				break;
			}
		}
	}
	else
		GenerateChunks();
}