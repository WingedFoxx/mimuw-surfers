// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SurferCharacter.h"
#include "BoosterPickup.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class MIMUWSURFERS_API ABoosterPickup : public AActor
{
    GENERATED_BODY()
    
public:    
    ABoosterPickup();

protected:
    virtual void BeginPlay() override;
    
    // This function runs in the Editor whenever you change a value
    virtual void OnConstruction(const FTransform& Transform) override;

public: 
    // -- COMPONENTS --
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* OverlapSphere;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* BoosterMesh;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* SphereCollider;

    // -- CONFIGURATION --
    // Choose if this is a Jump Boot or Score Multiplier
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Booster Settings")
    EBoosterType BoosterType;

    // How long the effect lasts (seconds)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Booster Settings")
    float Duration = 5.0f;

    // The strength (e.g., 1.5x Jump or 2x Score)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Booster Settings")
    float Multiplier = 2.0f;

    // -- VISUAL ASSETS --
    UPROPERTY(EditAnywhere, Category = "Visuals")
    UStaticMesh* JumpMeshAsset;

    UPROPERTY(EditAnywhere, Category = "Visuals")
    UStaticMesh* ScoreMeshAsset;

    // -- LOGIC --
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
                        bool bFromSweep, const FHitResult& SweepResult);
};