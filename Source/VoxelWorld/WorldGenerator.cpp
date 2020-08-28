// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldGenerator.h"
#include "Chunk.h"
#include "UObject/ConstructorHelpers.h"


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
	oldPlayerX = (int)floor(player->GetTransform().GetLocation().X / chunkSize);
	oldPlayerY = (int)floor(player->GetTransform().GetLocation().Y / chunkSize);

	//Texture.
	defaultAtlas = static_cast<FColor*>(defaultAtlas_UTexture2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_ONLY));
}

void AWorldGenerator::PostActorCreated()
{
	Super::PostActorCreated();
}

void AWorldGenerator::PostLoad()
{
	Super::PostLoad();
}

void AWorldGenerator::EndPlay()
{
	defaultAtlas_UTexture2D->PlatformData->Mips[0].BulkData.Unlock();
}

bool AWorldGenerator::loadChunk(FVector index)
{
	// Check if the chunk already is loaded.
	bool newChunk = true;
	for (auto c : chunks) {
		auto chunkIndex = c->chunkIndex;
		if (index == chunkIndex) {
			return false;
		}
	}

	// Create new chunk if it does not exist.
	FRotator rot = FRotator(0, 0, 0);
	auto pos = (index * chunkSize);
	auto v = (AChunk*)GetWorld()->SpawnActor(AChunk::StaticClass(), &pos, &rot);
	v->Initialize(index, this, material, defaultAtlas);
	v->SetFolderPath("/Chunks");
	chunks.Add(v);
	return true;
}

bool AWorldGenerator::doesChunkExist(FVector index, AChunk::ChunkStatus& _status)
{
	for (auto c : chunks) {
		if (c->chunkIndex == index) {
			_status = c->status;
			return true;
		}
	}
	return false;
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

	int playerPosX = (int)floor(player->GetTransform().GetLocation().X / chunkSize);
	int playerPosY = (int)floor(player->GetTransform().GetLocation().Y / chunkSize);

	// Player moves over chunk boundary.
	if (playerPosX != oldPlayerX || playerPosY != oldPlayerY) {
		oldPlayerX = playerPosX;
		oldPlayerY = playerPosY;
		stage = RenderStage::LOAD;
		ring = 1;
	}

	// Delete chunks
	for (auto c : chunks) {
		// Ads one extra chunk so that small movements over chunk boundaries does not delete/generate chunks.
		int maxDistance = renderDistance + (generationDistance * 2) + 1 + 1;
		int distX = abs(c->chunkIndex.X - playerPosX);
		int distY = abs(c->chunkIndex.Y - playerPosY);
		if (distX > maxDistance || distY > maxDistance) {
			chunks.Remove(c);
			c->Destroy();
			break;
		}
	}

	// End rendering when max render distance is reached.
	if (ring > renderDistance) {
		return;
	}

	//Render process.
	switch (stage)
	{
	case AWorldGenerator::LOAD:
	{

		int radius = ring + (generationDistance * 2) + 1;
		for (int z = 0; z <= numZChunks; z++)
			for (int x = -radius; x <= radius; x++)
				for (int y = -radius; y <= radius; y++) {
					FVector index = FVector(x + playerPosX, y + playerPosY, z);
					bool n = loadChunk(index);
					if (n)
						break;
				}
		break;
	}
	case AWorldGenerator::GENERATE:
	{
		int distanceToGenChunks = ring + generationDistance + 1;
		for (auto c : chunks) {
			int distX = abs(c->chunkIndex.X - playerPosX);
			int distY = abs(c->chunkIndex.Y - playerPosY);

			// Generate all chunks.
			if (ring == 1) { 
				if (distX <= distanceToGenChunks && distY <= distanceToGenChunks) {
					if (c->status == AChunk::ChunkStatus::GENERATE) {
						c->generateStructures();
						break;
					}
				}
			}
			else // Generate only the chunks in the middle of non-rendered chunks.
			{
				if ((distX == distanceToGenChunks && distY <= distanceToGenChunks) ||
					(distY == distanceToGenChunks && distX <= distanceToGenChunks)) {
					if (c->status == AChunk::ChunkStatus::GENERATE) {
						c->generateStructures();
						break;
					}
				}
			}

		}
		break;
	}
	case AWorldGenerator::DRAW:
	{
		for (auto c : chunks) {
			int distX = abs(c->chunkIndex.X - playerPosX);
			int distY = abs(c->chunkIndex.Y - playerPosY);
			if (distY <= ring && distX <= ring) {
				if (c->status == AChunk::ChunkStatus::DRAW) {
					c->RenderChunk();
					break;
				}
			}
		}
		break;
	}
	default:
		break;
	}

	//Check if ready for next stage.
	bool ready = true;
	switch (stage)
	{
	case AWorldGenerator::LOAD:
	{
		int radius = ring + (generationDistance * 2) + 1;
		for (int z = 0; z <= numZChunks; z++)
			for (int x = -radius; x <= radius; x++)
				for (int y = -radius; y <= radius; y++) {
					FVector index = FVector(x + playerPosX, y + playerPosY, z);
					AChunk::ChunkStatus s;
					bool m = doesChunkExist(index, s);
					if (m) {
						if (s == AChunk::ChunkStatus::LOAD) {
							ready = false;
						}
					}
				}

		if (ready) {
			stage = RenderStage::GENERATE;
		}
		break;
	}
	case AWorldGenerator::GENERATE:
	{
		int distanceToGenChunks = ring + generationDistance + 1;
		for (auto c : chunks) {
			int distX = abs(c->chunkIndex.X - playerPosX);
			int distY = abs(c->chunkIndex.Y - playerPosY);

			if ((distX == distanceToGenChunks && distY <= distanceToGenChunks) ||
				(distY == distanceToGenChunks && distX <= distanceToGenChunks)) {
				if (c->status != AChunk::ChunkStatus::DRAW && c->status != AChunk::ChunkStatus::DONE) {
					ready = false;
				}
			}
		}

		if (ready) {
			stage = RenderStage::DRAW;
		}
		break;
	}
	case AWorldGenerator::DRAW:
	{
		for (auto c : chunks) {
			int distX = abs(c->chunkIndex.X - playerPosX);
			int distY = abs(c->chunkIndex.Y - playerPosY);
			if (distY <= ring && distX <= ring) {
				if (c->status == AChunk::ChunkStatus::DRAW) {
					ready = false;
				}
			}
		}

		if (ready) {
			stage = RenderStage::LOAD;
			ring++;
		}
		break;
	}
	default:
		break;
	}



}