// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Coin.h"
#include "BaseLevel.generated.h"

class UBoxComponent;

UCLASS()
class MIMUWSURFERS_API ABaseLevel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseLevel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Coins")
	TSubclassOf<ACoin> CoinClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Coins")
	int32 CoinsNum = 15;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Coins")
	TArray<float> Lanes = {-200.f, 0.f, 200.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Coins")
	float CoinOffset = 100.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Triggers")
		UBoxComponent* Trigger;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Triggers")
		UBoxComponent* SpawnLocation;

public:
	UBoxComponent* GetTrigger();
	UBoxComponent* GetSpawnLocation();
};
