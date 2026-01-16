// Fill out your copyright notice in the Description page of Project Settings.

#include "Train.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

ATrain::ATrain()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create train body mesh
	TrainBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrainBody"));
	RootComponent = TrainBody;
	
	// Train body has regular collision (player can walk on top)
	TrainBody->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TrainBody->SetCollisionResponseToAllChannels(ECR_Block);

	// Create front collision box (deadly zone)
	FrontCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("FrontCollision"));
	FrontCollision->SetupAttachment(RootComponent);
	FrontCollision->SetBoxExtent(FVector(50.0f, 100.0f, 100.0f));  // Thin box at front
	FrontCollision->SetRelativeLocation(FVector(150.0f, 0.0f, 0.0f));  // Position at front of train
	FrontCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	FrontCollision->SetCollisionObjectType(ECC_GameTraceChannel1);  // Same as Spike
	FrontCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
	FrontCollision->SetGenerateOverlapEvents(true);

	// Create top collision (optional - for detecting player on top)
	TopCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("TopCollision"));
	TopCollision->SetupAttachment(RootComponent);
	TopCollision->SetBoxExtent(FVector(200.0f, 100.0f, 10.0f));  // Flat box on top
	TopCollision->SetRelativeLocation(FVector(0.0f, 0.0f, 110.0f));  // Position on top of train
	TopCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // Just for reference
}

void ATrain::BeginPlay()
{
	Super::BeginPlay();
	
	// Bind front collision overlap event
	FrontCollision->OnComponentBeginOverlap.AddDynamic(this, &ATrain::OnFrontOverlapBegin);
}

void ATrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATrain::OnFrontOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherOverlappedComponent, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
	// This is handled by SurferCharacter's OnOverlapBegin since FrontCollision
	// uses ECC_GameTraceChannel1 (same as Spike)
	// The character will detect this and restart the level
}
