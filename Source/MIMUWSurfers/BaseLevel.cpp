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

// Called when the game starts or when spawned
void ABaseLevel::BeginPlay()
{
	Super::BeginPlay();

	if (Trigger != nullptr)
		Trigger->bHiddenInGame = true;

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

		FVector Location(GetActorLocation().X + LevelBounds.X - i * Step, GetActorLocation().Y + Lanes[Lane], GetActorLocation().Z + 200.f);

		FHitResult Hit;
		FVector Start = Location;
		FVector End = Location - FVector(0,0,500.f);
		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(this);


		bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);
		if (bHit)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			ACoin* SpawnedCoin = GetWorld()->SpawnActor<ACoin>(CoinClass, Hit.Location + FVector(0,0,CoinOffset), FRotator::ZeroRotator, SpawnParams);
			if(SpawnedCoin)
			{
				SpawnedCoin->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			}
		}
	}
}

// Called every frame
void ABaseLevel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UBoxComponent* ABaseLevel::GetTrigger()
{
	return Trigger;
}

UBoxComponent* ABaseLevel::GetSpawnLocation()
{
	return SpawnLocation;
}
