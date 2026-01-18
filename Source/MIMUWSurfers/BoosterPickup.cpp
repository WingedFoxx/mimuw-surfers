// Fill out your copyright notice in the Description page of Project Settings.

#include "BoosterPickup.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "SurferCharacter.h" 
#include "Kismet/GameplayStatics.h"

ABoosterPickup::ABoosterPickup()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Create the Sphere Collider
    OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
    RootComponent = OverlapSphere;
    OverlapSphere->InitSphereRadius(50.0f);
    
    // Set Collision Profile to trigger overlap events
    OverlapSphere->SetCollisionProfileName(TEXT("Trigger"));

    // Create the Mesh (Visuals)
    BoosterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoosterMesh"));
    BoosterMesh->SetupAttachment(RootComponent);
    // Turn off collision for the mesh so only the sphere triggers events
    BoosterMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    
    SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
    SphereCollider->SetupAttachment(BoosterMesh);
    SphereCollider->SetSphereRadius(50.f);
    // The sphere is responsible for all overlap events.
    SphereCollider->SetCollisionProfileName(TEXT("OverlapAll"));
}

void ABoosterPickup::BeginPlay()
{
    Super::BeginPlay();
    SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &ABoosterPickup::OnOverlapBegin);
}

// Triggers whenever change of variables in the Editor is detected
void ABoosterPickup::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    // Automatically swap the mesh based on the Enum selection
    if (BoosterType == EBoosterType::HighJump && JumpMeshAsset)
    {
        BoosterMesh->SetStaticMesh(JumpMeshAsset);
    }
    else if (BoosterType == EBoosterType::DoubleScore && ScoreMeshAsset)
    {
        BoosterMesh->SetStaticMesh(ScoreMeshAsset);
    }
}

void ABoosterPickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
                                    bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->IsA(ASurferCharacter::StaticClass()))
    {
        // Check if it's the player
        ASurferCharacter* Player = Cast<ASurferCharacter>(OtherActor);
        
        // Activate the boost on the player
        Player->ActivateBooster(BoosterType, Duration, Multiplier);

        // Play Sound / Spawn Particle here if you want...
            
        // Destroy this pickup
        Destroy();
    }
}