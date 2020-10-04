// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldGenerator2.h"

#include "WorldSlice.h"
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
	
}

// Called every frame
void AWorldGenerator2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PlayerAtSlice.X = (int)floor(player->GetTransform().GetLocation().X / WORLD_PROPERTIES::ChunkRealSize);
	PlayerAtSlice.Y = (int)floor(player->GetTransform().GetLocation().Y / WORLD_PROPERTIES::ChunkRealSize);


	GenerateNewWorldSlices();

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

bool AWorldGenerator2::HasPlayerCrossedChunks()
{
	if (PlayerAtSlice.X != OldPlayerAtSlice.X || PlayerAtSlice.Y != OldPlayerAtSlice.Y)
		return true;
	else
		return false;
}

void AWorldGenerator2::DeleteUnnecessaryWorldSlices()
{
	// TODO.
}

void AWorldGenerator2::GenerateNewWorldSlices()
{
	for (int z = 0; z < WORLD_PROPERTIES::ChunksInHeight; z++)
		for (int x = -ActiveRenderDistance; x <= ActiveRenderDistance; x++)
			for (int y = -ActiveRenderDistance; y <= ActiveRenderDistance; y++) {
				
				FVector2D WorldSliceindex = FVector2D(x + PlayerAtSlice.X, y + PlayerAtSlice.Y);
					
				if (!DoesWorldSliceExist(WorldSliceindex)) {
					GenerateWorldSlice(WorldSliceindex);
					WorldSlices[WorldSliceindex]->RenderChunks();
					break;
				}
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

