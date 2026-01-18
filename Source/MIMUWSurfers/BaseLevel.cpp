// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseLevel.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABaseLevel::ABaseLevel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ABaseLevel::PlaceCoins()
{
	if (CoinClass == nullptr || CoinsNum <= 0 || Lanes.Num() == 0)
	{
		return;
	}

	FVector LevelOrigin;
	FVector LevelBounds;
	GetActorBounds(false, LevelOrigin, LevelBounds);

	const float Step = LevelBounds.X * 2 / CoinsNum;

	for (int i = 0; i < CoinsNum; i++)
	{
		const int Lane = UKismetMathLibrary::RandomIntegerInRange(0, Lanes.Num() - 1);

		// FIX 1: Raise the Z value (e.g., +1000) so the trace starts ABOVE the train.
		FVector Location(GetActorLocation().X + LevelBounds.X - i * Step, GetActorLocation().Y + Lanes[Lane], GetActorLocation().Z + 1000.f);

		FHitResult Hit;
		FVector Start = Location;
        
		// FIX 2: Increase the trace length (e.g., 2000) so it reaches the floor from the new height.
		FVector End = Location - FVector(0, 0, 2000.f);
        
		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(this);

		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams))
		{
			// Use the exact hit location (whether it's the train roof or the floor)
			float CoinZ = Hit.Location.Z + CoinOffset;

			FVector CoinLocation = FVector(Hit.Location.X, Hit.Location.Y, CoinZ);

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
            
			if (ACoin* SpawnedCoin = GetWorld()->SpawnActor<ACoin>(CoinClass, CoinLocation, FRotator::ZeroRotator, SpawnParams))
			{
				SpawnedCoin->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			}
		}
	}
}

// Called when the game starts or when spawned
void ABaseLevel::BeginPlay()
{
	Super::BeginPlay();

	if (Trigger != nullptr)
		Trigger->bHiddenInGame = true;

	PlaceCoins();
}

// Called every frame
void ABaseLevel::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UBoxComponent* ABaseLevel::GetTrigger() const
{
	return Trigger;
}

UBoxComponent* ABaseLevel::GetSpawnLocation() const
{
	return SpawnLocation;
}
