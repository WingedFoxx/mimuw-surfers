// Fill out your copyright notice in the Description page of Project Settings.


#include "SurferCharacter.h"

#include "Spike.h"
#include "Train.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SurferSaveGame.h"
#include "Kismet/GameplayStatics.h"

#include "Engine.h"

// Sets default values
ASurferCharacter::ASurferCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionResponseToChannel
	(ECC_GameTraceChannel1, ECR_Overlap);
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	SideViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Side View Camera"));
	SideViewCamera->SetFieldOfView(110.0f);
	SideViewCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.f, 0.0f);
	
	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->AirControl = 0.8f;
	GetCharacterMovement()->JumpZVelocity = 1000.0f;
	GetCharacterMovement()->GroundFriction = 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;
	GetCharacterMovement()->SetWalkableFloorAngle(75.0f);  // Allow walking up steeper ramps
}

// Called when the game starts or when spawned
void ASurferCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	StartingXPosition = GetActorLocation().X;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(
		this, &ASurferCharacter::OnOverlapBegin);

	CanMove = true;

	// Store initial character size / collision height
	DefaultCapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	
	// Initialize lane position (start in center lane)
	CurrentLane = 1;
	TargetLaneY = GetActorLocation().Y;
	
	// Store the initial forward-facing rotation
	BaseRotationYaw = GetActorRotation().Yaw;
	TargetRotationYaw = BaseRotationYaw;
	
	// Initialize camera height
	TargetCameraZ = GetActorLocation().Z + CameraOffset;
	BaseGroundZ = GetActorLocation().Z;
	LastGroundedZ = BaseGroundZ;

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
    
    if (PlayerController)
    {
        // Hide the mouse cursor
        // PlayerController->bShowMouseCursor = false;

        // Tell Input to focus on the Game (Character) again, not UI
        FInputModeGameOnly InputMode;
        PlayerController->SetInputMode(InputMode);
    }
	
	// Load the High Score from disk
	LoadHighScore();
}

void ASurferCharacter::LoadHighScore()
{
	// Check if a save file exists
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		// Load it
		USurferSaveGame* LoadInstance = Cast<USurferSaveGame>(
			UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));

		if (LoadInstance)
		{
			HighScore = LoadInstance->HighScore;
			// Debug log to verify
			UE_LOG(LogTemp, Warning, TEXT("Loaded High Score: %f"), HighScore);
		}
	}
	else
	{
		// No save found, this is a new player
		HighScore = 0.0f;
	}
}

void ASurferCharacter::CheckAndSaveHighScore()
{
	// Only save if we beat the record
	if (Score > HighScore)
	{
		HighScore = Score;

		// Create a new SaveGame object instance
		USurferSaveGame* SaveGameInstance = Cast<USurferSaveGame>(
			UGameplayStatics::CreateSaveGameObject(USurferSaveGame::StaticClass()));

		if (SaveGameInstance)
		{
			// Set data
			SaveGameInstance->HighScore = HighScore;

			// Write to disk
			UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, 0);

			UE_LOG(LogTemp, Warning, TEXT("New High Score Saved: %f"), HighScore);
		}
	}
}

void ASurferCharacter::HandleLaneSwitching(float DeltaTime)
{
	// Smooth lane switching
	if (bIsSwitchingLane)
	{
		FVector CurrentLocation = GetActorLocation();
		float NewY = FMath::FInterpTo(CurrentLocation.Y, TargetLaneY, DeltaTime, LaneSwitchSpeed);
		SetActorLocation(FVector(CurrentLocation.X, NewY, CurrentLocation.Z));
		
		// Check if we've reached the target lane
		if (FMath::IsNearlyEqual(NewY, TargetLaneY, 1.0f))
		{
			bIsSwitchingLane = false;
			TargetRotationYaw = BaseRotationYaw;  // Return to forward facing
		}
	}
}

void ASurferCharacter::MoveCameraWhenFalling()
{
	// Prevent camera from moving while dodging
	if (!isDodging) {
		// Track if we're on an elevated surface (like a train)
		if (!GetCharacterMovement()->IsFalling())
		{
			LastGroundedZ = GetActorLocation().Z;
			// Consider elevated if more than 50 units above base ground
			bWasOnElevatedSurface = (LastGroundedZ > BaseGroundZ + 50.0f);
			TargetCameraZ = GetActorLocation().Z + CameraOffset;
		}
		else if (bWasOnElevatedSurface && GetVelocity().Z < 0)
		{
			// Only follow camera down when falling OFF the train (below where we were standing)
			// Don't follow if we're still above the train (jumping on top of it)
			if (GetActorLocation().Z < LastGroundedZ - 10.0f)
			{
				TargetCameraZ = GetActorLocation().Z + CameraOffset;
			}
		}
	}
}

// Called every frame
void ASurferCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Constant forward running
	if (CanMove)
	{
		AddMovementInput(FVector(1.0f, 0.0f, 0.0f), RunSpeed * DeltaTime);
		Score = FMath::Max(0.0f, GetActorLocation().X / 10.0f);
	}

	HandleLaneSwitching(DeltaTime);
	
	// Smooth rotation interpolation
	FRotator CurrentRotation = GetActorRotation();
	float NewYaw = FMath::FInterpTo(CurrentRotation.Yaw, TargetRotationYaw, DeltaTime, RotationInterpSpeed);
	SetActorRotation(FRotator(CurrentRotation.Pitch, NewYaw, CurrentRotation.Roll));

	// Camera positioning
	tempPos = GetActorLocation();
	tempPos.X -= 850.0f;
	
	MoveCameraWhenFalling();
	// Camera falls faster than it rises (quicker descent from trains)
	float CurrentCameraZ = SideViewCamera->GetComponentLocation().Z;
	float CameraInterpSpeed = (TargetCameraZ < CurrentCameraZ) ? 15.0f : 5.0f;  // Fall fast, rise slow
	tempPos.Z = FMath::FInterpTo(CurrentCameraZ, TargetCameraZ, DeltaTime, CameraInterpSpeed);
	
	SideViewCamera->SetWorldLocation(tempPos);
}

// Called to bind functionality to input
void ASurferCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	
	// Lane switching
	PlayerInputComponent->BindAction("SwitchLaneLeft", IE_Pressed, this, &ASurferCharacter::SwitchLaneLeft);
	PlayerInputComponent->BindAction("SwitchLaneRight", IE_Pressed, this, &ASurferCharacter::SwitchLaneRight);
	
	// Dodge binding
	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &ASurferCharacter::Dodge);
}

void ASurferCharacter::SwitchLaneLeft()
{
	if (!CanMove) return;
	
	// Check if we can go left
	if (CurrentLane > MinLane)
	{
		CurrentLane--;
		TargetLaneY -= LaneDistance;
		bIsSwitchingLane = true;
		TargetRotationYaw = BaseRotationYaw - LaneSwitchTurnAngle;  // Turn left
		UE_LOG(LogTemp, Warning, TEXT("Switching to lane: %d"), CurrentLane);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Already at leftmost lane!"));
	}
}

void ASurferCharacter::SwitchLaneRight()
{
	if (!CanMove) return;
	
	// Check if we can go right
	if (CurrentLane < MaxLane)
	{
		CurrentLane++;
		TargetLaneY += LaneDistance;
		bIsSwitchingLane = true;
		TargetRotationYaw = BaseRotationYaw + LaneSwitchTurnAngle;  // Turn right
		bIsSwitchingLane = true;
		UE_LOG(LogTemp, Warning, TEXT("Switching to lane: %d"), CurrentLane);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Already at rightmost lane!"));
	}
}

void ASurferCharacter::Dodge()
{
	if (isDodging || !CanMove) return;

	// CanJump to make sure we don't dodge mid-air
	if (CanJump() && dodgeMontage)
	{
		float Duration = PlayAnimMontage(dodgeMontage);
		isDodging = true;

		// Shrink the Collision Capsule to half height
        // The "true" argument updates overlapping physics immediately
        GetCapsuleComponent()->SetCapsuleHalfHeight(DefaultCapsuleHalfHeight / 2.0f, true);

		// Optional: Reset isDodging after the animation finishes
		// A timer or Animation Notify is usually better, but for simplicity:
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ASurferCharacter::StopDodge, Duration, false);
	}
}

void ASurferCharacter::StopDodge()
{
	isDodging = false;
	
	// Restore the collision size
    GetCapsuleComponent()->SetCapsuleHalfHeight(DefaultCapsuleHalfHeight, true);
}

void ASurferCharacter::RestartLevel()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()));
}

void ASurferCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherOverlappedComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr)
	{
		// Debug: Print what actor we're overlapping with
		UE_LOG(LogTemp, Warning, TEXT("Overlap with: %s"), *OtherActor->GetName());
		
		ASpike* Spike = Cast<ASpike>(OtherActor);
		ATrain* Train = Cast<ATrain>(OtherActor);

		if (Spike || Train)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit a spike! Restarting..."));
			
			GetMesh()->Deactivate();
			GetMesh()->SetVisibility(false);

			CanMove = false;

			CheckAndSaveHighScore();
			
			// Play hit sound if assigned
			if (HitObstacleSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, HitObstacleSound, GetActorLocation());
			}
			
			if (GameOverWidgetClass)
			{
				// 1. Create the Widget
				UUserWidget* GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
            
				if (GameOverWidget)
				{
					// 2. Add to Screen
					GameOverWidget->AddToViewport();

					// 3. Enable Mouse Cursor so player can click "Restart"
					APlayerController* PC = Cast<APlayerController>(GetController());
					if (PC)
					{
						PC->bShowMouseCursor = true;
                    
						// Input Mode: UI Only (Stops player from moving character with keys)
						FInputModeUIOnly InputMode;
						InputMode.SetWidgetToFocus(GameOverWidget->TakeWidget());
						PC->SetInputMode(InputMode);
					}
				}
			}
			// FTimerHandle UnusedHandle;
			// GetWorldTimerManager().SetTimer(UnusedHandle,
			// 	this, &ASurferCharacter::RestartLevel, 2.f, false);
		}
	}
}

