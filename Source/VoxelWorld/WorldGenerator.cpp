// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldGenerator.h"
#include "Chunk.h"
#include "InternalChunkData.h"


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
	GenerateChunks();
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
	isBuilding = true;
	FRotator rot = FRotator(0, 0, 0);
	int playerPosX = (int) floor(player->GetTransform().GetLocation().X / chunkSize);
	int playerPosY = (int) floor(player->GetTransform().GetLocation().Y / chunkSize);

	UWorld* WRLD = GetWorld();
	// Load chunks and change chunk status.
	if (GetWorld()) {
		for (int x = -renderRadius; x <= renderRadius; x++) {
			for (int y = -renderRadius; y <= renderRadius; y++) {

				FVector pos = FVector((x + playerPosX)*chunkSize, (y + playerPosY) * chunkSize, 0);
				FVector index = FVector(x + playerPosX, y + playerPosY, 0);

				auto distance = FVector(index.X - playerPosX, index.Y - playerPosY, 0).Size();
				if (distance > renderRadius)
					continue;

				// Check if the chunk already is loaded.
				bool newChunk = true;
				for (auto c : chunks) {
					auto chunkIndex = c->chunkIndex;
					if (index == chunkIndex) {
						newChunk = false;
						//c->status = AChunk::ChunkStatus::KEEP; //usefull if destroy chunks will be based on this?
					}
				}

				// Create new chunk if it does not exist.
				if (newChunk) {
					auto v = (AChunk*)GetWorld()->SpawnActor(AChunk::StaticClass(), &pos, &rot);
					v->SetFolderPath("/Chunks");
					chunks.Add(v);
					v->Initialize(index, voxelsInChunkXYZ, voxelsInChunkXYZ, this);
				}
			}
		}
	}

	//Remove chunks
	for (size_t i = 0; i < chunks.Num(); i++) {
		auto c = chunks[i];
		auto distance = FVector(c->chunkIndex.X - playerPosX, c->chunkIndex.Y - playerPosY, 0).Size();
		if (distance > renderRadius + 1) {
			chunks.Remove(c);
			c->Destroy();
			i--;
		}
	}

	// Render chunks
	for (auto c : chunks) {
		if (c->status == AChunk::ChunkStatus::DRAW) {
			c->RenderChunk();
			c->status = AChunk::ChunkStatus::DONE;
		}
	}

	isBuilding = false;
}



// Called every frame
void AWorldGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!isBuilding) {
		GenerateChunks(); //game main.
		//(new FAutoDeleteAsyncTask<ChunkTask>(this))->StartBackgroundTask(); //tread. Does not work.
	}
}


//===========================CHUNKTASK==============
ChunkTask::ChunkTask(AWorldGenerator* w)
{
	worldG = w;
}

void ChunkTask::DoWork()
{
	worldG->isBuilding = true;
	FRotator rot = FRotator(0, 0, 0);
	int playerPosX = (int)floor(worldG->player->GetTransform().GetLocation().X / worldG->chunkSize);
	int playerPosY = (int)floor(worldG->player->GetTransform().GetLocation().Y / worldG->chunkSize);

	// Load chunks and change chunk status.
	UWorld* WRLD = worldG->GetWorld();
	if (WRLD) {
		for (int x = -worldG->renderRadius; x <= worldG->renderRadius; x++) {
			for (int y = -worldG->renderRadius; y <= worldG->renderRadius; y++) {

				FVector pos = FVector((x + playerPosX) * worldG->chunkSize, (y + playerPosY) * worldG->chunkSize, 0);
				FVector index = FVector(x + playerPosX, y + playerPosY, 0);

				auto distance = FVector(index.X - playerPosX, index.Y - playerPosY, 0).Size();
				if (distance > worldG->renderRadius)
					continue;

				// Check if the chunk already is loaded.
				bool newChunk = true;
				for (auto c : worldG->chunks) {
					auto chunkIndex = c->chunkIndex;
					if (index == chunkIndex) {
						newChunk = false;
						//c->status = AChunk::ChunkStatus::KEEP; //usefull if destroy chunks will be based on this?
					}
				}

				// Create new chunk if it does not exist.
				if (newChunk) {
					auto v = (AChunk*)WRLD->SpawnActor(AChunk::StaticClass(), &pos, &rot);
					v->SetFolderPath("/Chunks");
					worldG->chunks.Add(v);
					v->Initialize(index, worldG->voxelsInChunkXYZ, worldG->voxelsInChunkXYZ, worldG);
				}
			}
		}
	}

	//Remove chunks
	for (size_t i = 0; i < worldG->chunks.Num(); i++) {
		auto c = worldG->chunks[i];
		auto distance = FVector(c->chunkIndex.X - playerPosX, c->chunkIndex.Y - playerPosY, 0).Size();
		if (distance > worldG->renderRadius + 1) {
			worldG->chunks.Remove(c);
			c->Destroy();
			i--;
		}
	}

	// Render chunks
	for (auto c : worldG->chunks) {
		if (c->status == AChunk::ChunkStatus::DRAW) {
			c->RenderChunk();
			c->status = AChunk::ChunkStatus::DONE;
		}
	}

	worldG->isBuilding = false;
}
