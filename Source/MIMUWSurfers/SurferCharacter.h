// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Blueprint/UserWidget.h"
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
	void HandleLaneSwitching(float DeltaTime);
	void MoveCameraWhenFalling();
	
	float StartingXPosition;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// The Game Over Widget Class 
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;
	
	// Score and Highest score
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	float Score = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	float HighScore = 0.0f;
	
protected:
	// Constant name for the save file
	FString SaveSlotName = "SurferSaveSlot";

	// Function to handle Loading
	void LoadHighScore();

	// Function to handle Saving
	void CheckAndSaveHighScore();
	
public:
	class UCameraComponent* GetFrontViewCameraComponent() const
	{
		return SideViewCamera;
	}

	void RestartLevel();

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherOverlappedComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Dodging
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* dodgeMontage;	
	bool isDodging = false;
	void Dodge();

	// Lane switching
	void SwitchLaneLeft();
	void SwitchLaneRight();

private:
	FVector tempPos = FVector(0.0f, 0.0f, 0.0f);
	float TargetCameraZ = 0.0f;  // Target camera height (only updates when grounded)
	
	// Track if player was on elevated surface (train) before falling
	float BaseGroundZ = 0.0f;  // Normal ground level
	float LastGroundedZ = 0.0f;  // Z when player was last grounded
	bool bWasOnElevatedSurface = false;

	bool CanMove;
	
	// Player collision height
	float DefaultCapsuleHalfHeight;
	
	// Auto running
	UPROPERTY(EditAnywhere, Category = "Movement")
	float RunSpeed = 1000.0f;

	// Lane system (0 = left, 1 = center, 2 = right)
	int32 CurrentLane = 1;
	const int32 MinLane = 0;
	const int32 MaxLane = 2;

	float CameraOffset = 350.0f;
	
	// Distance between lanes (adjust based on your level)
	UPROPERTY(EditAnywhere, Category = "Lane")
	float LaneDistance = 200.0f;
	
	// Target Y position for smooth lane switching
	float TargetLaneY;
	bool bIsSwitchingLane = false;
	
	UPROPERTY(EditAnywhere, Category = "Lane")
	float LaneSwitchSpeed = 15.0f;
	
	// Lane switch rotation (visual turn)
	UPROPERTY(EditAnywhere, Category = "Lane")
	float LaneSwitchTurnAngle = 60.0f;  // How much to turn when switching
	
	float TargetRotationYaw = 0.0f;
	float BaseRotationYaw = 0.0f;  // The forward-facing rotation
	
	UPROPERTY(EditAnywhere, Category = "Lane")
	float RotationInterpSpeed = 20.0f;
	
	void StopDodge();

public:
	// Sound to play when hitting obstacle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* HitObstacleSound = nullptr;
};
