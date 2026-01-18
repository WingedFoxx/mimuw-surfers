// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelSpawnerLevelList.h"
#include "BaseLevel.h"
#include "Engine.h"
#include "Components/BoxComponent.h"
#include "SurferCharacter.h" // Include the header for the player character class

// Sets default values
ALevelSpawnerLevelList::ALevelSpawnerLevelList()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALevelSpawnerLevelList::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnLevel(true);
	SpawnLevel(false);
	SpawnLevel(false);
	SpawnLevel(false);
}

// Called every frame
void ALevelSpawnerLevelList::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelSpawnerLevelList::SpawnLevel(bool IsFirst)
{
	SpawnLocation = FVector(1000.0f, 0.0f, 0.0f);
	SpawnRotation = FRotator(0.0f, 90.0f, 0.0f);

	if (!IsFirst && SpawnedLevels.Num() > 0)
	{
		ABaseLevel* LastLevel = SpawnedLevels.Last();
		SpawnLocation = LastLevel->GetSpawnLocation()->GetComponentTransform().GetTranslation();
	}
    
	// Safety check: ensure the array isn't empty to prevent crashing
	if (Levels.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Levels set in the LevelSpawner Blueprint!"));
		return;
	}

	// Get a random index valid for the current array size
	RandomLevelIndex = FMath::RandRange(0, Levels.Num() - 1);
    
	// Pick the class from the array
	TSubclassOf<ABaseLevel> SelectedLevelClass = Levels[RandomLevelIndex];

	ABaseLevel* NewLevel = nullptr;

	if (SelectedLevelClass)
	{
		NewLevel = GetWorld()->SpawnActor<ABaseLevel>(SelectedLevelClass, SpawnLocation, SpawnRotation, SpawnInfo);
	}

	if (NewLevel)
	{
		if (NewLevel->GetTrigger())
		{
			NewLevel->GetTrigger()->OnComponentBeginOverlap.AddDynamic(this, &ALevelSpawnerLevelList::OnOverlapBegin);
		}
        
		SpawnedLevels.Add(NewLevel);
	}

	// Optimization: Destroy the old level to free up memory
	if (SpawnedLevels.Num() > 10)
	{
		ABaseLevel* OldLevel = SpawnedLevels[0];
		if (OldLevel)
		{
			OldLevel->Destroy(); // Actually remove the actor from the game world
		}
		SpawnedLevels.RemoveAt(0); // Remove pointer from list
	}
}

void ALevelSpawnerLevelList::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherOverlappedComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Only spawn a new level if the player character overlaps
	if (OtherActor && OtherActor->IsA(ASurferCharacter::StaticClass()))
	{
		SpawnLevel(false);
	}
}

