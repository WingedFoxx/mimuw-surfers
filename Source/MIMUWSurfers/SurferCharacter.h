// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SurferCharacter.generated.h"

UCLASS()
class MIMUWSURFERS_API ASurferCharacter : public ACharacter
{
	GENERATED_BODY()
		UPROPERTY(VisibleAnywhere)
		class UCameraComponent* SideViewCamera;

public:
	// Sets default values for this character's properties
	ASurferCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void MoveRight(float Value);

public:
	class UCameraComponent* GetFrontViewCameraComponent() const
	{
		return SideViewCamera;
	}

	void RestartLevel();

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherOverlappedComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	float zPosition;
	FVector tempPos = FVector(0.0f, 0.0f, 0.0f);

	bool CanMove;
};
