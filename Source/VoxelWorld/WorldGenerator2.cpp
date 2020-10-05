// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldGenerator2.h"

#include "WorldSlice.h"
#include "WorldProperties.h"
#include "TerrainNoise.h"

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

StructureData AWorldGenerator2::GetStructureData(FVector2D Index)
{
	StructureGen.GenerateArea(PlayerAtSlice);
	return StructureGen.GetData(Index);
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
	UTerrainNoise* noise = NewObject<UTerrainNoise>(TerrainNoise);

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

