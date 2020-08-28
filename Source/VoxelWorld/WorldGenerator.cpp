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

	readTexute();
}

void AWorldGenerator::readTexute()
{
	int TextureInFileSize = defaultAtlas_UTexture2D->GetSizeX(); //the width of the texture
	colorAtlas.Init(FColor(0, 0, 0, 255), TextureInFileSize * TextureInFileSize);//making sure it has something, and sizing the array n*n
	//init TArray
	//What i want to do is take all the values from Texture File ->to-> TArray of FColors
	if (!defaultAtlas_UTexture2D) {
		//Many times i forgot to load the texture in the editor so every time i hit play the editor crashed
		UE_LOG(LogTemp, Error, TEXT("Missing texture in LevelInfo, please load the mask!"));
		return; //<---if textureInFile is missing stop execution of the code
	}
	if (defaultAtlas_UTexture2D != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Texture in file is :  %d  pixels wide"), TextureInFileSize);

		FTexture2DMipMap& Mip = defaultAtlas_UTexture2D->PlatformData->Mips[0];//A reference 
		void* Data = Mip.BulkData.Lock(LOCK_READ_WRITE);
		uint8* raw = NULL;
		raw = (uint8*)Data;

		FColor pixel = FColor(0, 0, 0, 255);//used for spliting the data stored in raw form

		//the usual nested loop for reading the pixels
		for (int y = 0; y < TextureInFileSize; y++) {

			for (int x = 0; x < TextureInFileSize; x++) {
				//data in the raw var is serialized i think ;)
				//so a pixel is four consecutive numbers e.g 0,0,0,255
				//and the following code split the values in single components and store them in a FColor
				pixel.B = raw[4 * (TextureInFileSize * y + x) + 0];
				pixel.G = raw[4 * (TextureInFileSize * y + x) + 1];
				pixel.R = raw[4 * (TextureInFileSize * y + x) + 2];
				//And then this code iterates over the TArray of FColors and stores them
				colorAtlas[x + y * TextureInFileSize] = FColor((uint8)pixel.R, (uint8)pixel.G, (uint8)pixel.B, 255);
			}
		}
		Mip.BulkData.Unlock();
		defaultAtlas_UTexture2D->UpdateResource();
	}
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
	v->Initialize(index, this, material, &colorAtlas);
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

bool AWorldGenerator::isReadyForNextStage() {
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
							return false;
						}
					}
					else {
						return false;
					}
				}

		return true;
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
					return false;
				}
			}
		}

		return true;
		break;
	}
	case AWorldGenerator::DRAW:
	{
		for (auto c : chunks) {
			int distX = abs(c->chunkIndex.X - playerPosX);
			int distY = abs(c->chunkIndex.Y - playerPosY);
			if (distY <= ring && distX <= ring) {
				if (c->status == AChunk::ChunkStatus::DRAW) {
					return false;
				}
			}
		}
		return true;
		break;
	}
	default:
		return false;
		break;
	}
}

// Called every frame
void AWorldGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	playerPosX = (int)floor(player->GetTransform().GetLocation().X / chunkSize);
	playerPosY = (int)floor(player->GetTransform().GetLocation().Y / chunkSize);

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

	if (isReadyForNextStage()) {
		switch (stage)
		{
		case AWorldGenerator::LOAD:
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Load Chunk end."));
			stage = RenderStage::GENERATE;
			break;
		case AWorldGenerator::GENERATE:
			stage = RenderStage::DRAW;
			break;
		case AWorldGenerator::DRAW:
			stage = RenderStage::LOAD;
			ring++;
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Load Chunk start."));
			break;
		case AWorldGenerator::NONE:
			break;
		default:
			break;
		}
	}
}