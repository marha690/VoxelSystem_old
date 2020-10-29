// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldGenerator2.h"

#include "WorldSlice.h"
#include "WorldProperties.h"
#include "TerrainNoise.h"
#include "WorldProperties.h"

// Sets default values
AWorldGenerator2::AWorldGenerator2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWorldGenerator2::BeginPlay()
{
	Super::BeginPlay();

	WRLD = GetWorld(); // Get current world.
	verify(WRLD != nullptr);
	
	StructureGen = StructuresGenerator(renderDistance);
	readTexute();
}

// Called every frame
void AWorldGenerator2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PlayerAtSlice.X = (int)floor(player->GetTransform().GetLocation().X / WORLD_PROPERTIES::ChunkRealSize);
	PlayerAtSlice.Y = (int)floor(player->GetTransform().GetLocation().Y / WORLD_PROPERTIES::ChunkRealSize);


	GenerateNewWorldSlices();
	GenerateTerrainNoise();

	if(ActiveRenderDistance < renderDistance) {
		++ActiveRenderDistance; 
	}

	if (HasPlayerCrossedChunks()) {
		ActiveRenderDistance = 1; // Reset render distance.
		DeleteUnnecessaryWorldSlices();
		StructureGen.DeleteGenerationsInSlices(PlayerAtSlice); // Remove structures far away.
		OldPlayerAtSlice = PlayerAtSlice;
	}
}

AWorldSlice* AWorldGenerator2::GetWorldSlice(FVector2D WSI)
{
	if (WorldSlices.Contains(WSI)) {
		return WorldSlices[WSI];
	}
	else
		return nullptr;
}

const StructureData& AWorldGenerator2::GetStructureData(FVector2D Index)
{
	return StructureGen.GetData(Index);
}

void AWorldGenerator2::readTexute()
{
	if (!defaultAtlas_UTexture2D) {
		//TODO. Use basic texture atlas.
		hasColorAtlas = false;
		return;
	}

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
	hasColorAtlas = true;
}

bool AWorldGenerator2::HasPlayerCrossedChunks()
{
	if (PlayerAtSlice.X != OldPlayerAtSlice.X || PlayerAtSlice.Y != OldPlayerAtSlice.Y)
		return true;
	else
		return false;
}

void AWorldGenerator2::DeleteUnnecessaryWorldSlices()
{
	for (auto It = WorldSlices.CreateConstIterator(); It; ++It)
	{
		int x = It.Value()->SlicePositionIndex.X;
		int y = It.Value()->SlicePositionIndex.Y;

		if (abs(x - PlayerAtSlice.X) > renderDistance ||
			abs(y - PlayerAtSlice.Y) > renderDistance) {

			WorldSlices.Remove(It.Key());
			It.Value()->Destroy();
		}
	}
}

void AWorldGenerator2::GenerateNewWorldSlices()
{
	for (int x = -ActiveRenderDistance - 1; x <= ActiveRenderDistance + 1; x++)
		for (int y = -ActiveRenderDistance - 1; y <= ActiveRenderDistance + 1; y++) {
			FVector2D WorldSliceIndex = FVector2D(x + PlayerAtSlice.X, y + PlayerAtSlice.Y);
			if (!DoesWorldSliceExist(WorldSliceIndex)) {
				GenerateWorldSlice(WorldSliceIndex);
			}
		}
}

void AWorldGenerator2::GenerateTerrainNoise()
{
	auto noise = Cast<UTerrainNoise>(TerrainNoise);

	// Generate terrain
	for (int x = -ActiveRenderDistance; x <= ActiveRenderDistance - 1; x++)
		for (int y = -ActiveRenderDistance; y <= ActiveRenderDistance - 1; y++) {
			FVector2D WorldSliceIndex = FVector2D(x + PlayerAtSlice.X, y + PlayerAtSlice.Y);

			if (DoesWorldSliceExist(WorldSliceIndex) && !WorldSlices[WorldSliceIndex]->isTerrainGenerated) {
				WorldSlices[WorldSliceIndex]->isTerrainGenerated = true;
				WorldSlices[WorldSliceIndex]->GenerateTerrainFromNoise(noise->generate2DHeightMap);
				WorldSlices[WorldSliceIndex]->GenerateStructures();
				break;
			}
		}


	// Generate trees?
	for (int x = -ActiveRenderDistance + 1; x <= ActiveRenderDistance - 2; x++)
		for (int y = -ActiveRenderDistance + 1; y <= ActiveRenderDistance - 2; y++) {
			FVector2D WorldSliceIndex = FVector2D(x + PlayerAtSlice.X, y + PlayerAtSlice.Y);

			if (DoesWorldSliceExist(WorldSliceIndex)) {
					WorldSlices[WorldSliceIndex]->GenerateObjects();
			}
		}

	// Render mesh
	for (int x = -ActiveRenderDistance; x <= ActiveRenderDistance - 1; x++)
		for (int y = -ActiveRenderDistance; y <= ActiveRenderDistance - 1; y++) {
			FVector2D WorldSliceIndex = FVector2D(x + PlayerAtSlice.X, y + PlayerAtSlice.Y);
			FVector2D WorldSliceIndexPx = FVector2D(x + PlayerAtSlice.X + 1, y + PlayerAtSlice.Y);
			FVector2D WorldSliceIndexMx = FVector2D(x + PlayerAtSlice.X - 1, y + PlayerAtSlice.Y);
			FVector2D WorldSliceIndexPy = FVector2D(x + PlayerAtSlice.X, y + PlayerAtSlice.Y + 1);
			FVector2D WorldSliceIndexMy = FVector2D(x + PlayerAtSlice.X, y + PlayerAtSlice.Y - 1);

			if (DoesWorldSliceExist(WorldSliceIndex) && !WorldSlices[WorldSliceIndex]->isRendered && WorldSlices[WorldSliceIndexPx]->isTerrainGenerated)
				if (DoesWorldSliceExist(WorldSliceIndexPx) && WorldSlices[WorldSliceIndexPx]->isTerrainGenerated)
					if (DoesWorldSliceExist(WorldSliceIndexMx) && WorldSlices[WorldSliceIndexMx]->isTerrainGenerated)
						if (DoesWorldSliceExist(WorldSliceIndexPy) && WorldSlices[WorldSliceIndexPy]->isTerrainGenerated)
							if (DoesWorldSliceExist(WorldSliceIndexMy) && WorldSlices[WorldSliceIndexMy]->isTerrainGenerated)
								WorldSlices[WorldSliceIndex]->RenderChunks();
		}
}

bool AWorldGenerator2::DoesWorldSliceExist(FVector2D WSI)
{
	if (WorldSlices.Contains(WSI)) {
		return true;
	}
	return false;
}

void AWorldGenerator2::GenerateWorldSlice(FVector2D WSI)
{
	FRotator rot = FRotator(0, 0, 0);
	auto worldPos = (FVector(WSI.X, WSI.Y, 0.f) * WORLD_PROPERTIES::ChunkRealSize);
	auto v = (AWorldSlice*)WRLD->SpawnActor(AWorldSlice::StaticClass(), &worldPos, &rot);
	v->Initialize(WSI, this);
	v->SetFolderPath("/WorldSlices");
	WorldSlices.Add(WSI, v);
}

