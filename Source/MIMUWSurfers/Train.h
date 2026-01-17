// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Train.generated.h"

UCLASS()
class MIMUWSURFERS_API ATrain : public AActor
{
	GENERATED_BODY()
	
public:	
	ATrain();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	// Main train body - player can walk on top
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* TrainBody;

	// Front collision - hitting this kills the player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* FrontCollision;

public:
	UFUNCTION()
	void OnFrontOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherOverlappedComponent, int32 OtherBodyIndex, 
		bool bFromSweep, const FHitResult& SweepResult);

	// Getters
	UBoxComponent* GetFrontCollision() const { return FrontCollision; }
};
