// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelSpawnerLevelList.generated.h"

class ABaseLevel;

UCLASS()
class MIMUWSURFERS_API ALevelSpawnerLevelList : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelSpawnerLevelList();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void SpawnLevel(bool IsFirst);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor, UPrimitiveComponent* OtherOverlappedComponent,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	APawn* Player;
	UPROPERTY(EditAnywhere, Category = "Levels")
	TArray<TSubclassOf<ABaseLevel>> Levels;
	TArray<ABaseLevel*> SpawnedLevels;

public:
	int RandomLevelIndex;
	FVector SpawnLocation = FVector();
	FRotator SpawnRotation = FRotator();
	FActorSpawnParameters SpawnInfo = FActorSpawnParameters();
};
