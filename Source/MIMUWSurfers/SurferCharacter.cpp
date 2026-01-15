// Fill out your copyright notice in the Description page of Project Settings.


#include "SurferCharacter.h"

#include "Spike.h"
#include "WallSpike.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Spike.h"
#include "WallSpike.h"
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
	SideViewCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.f, 0.0f);
	
	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->AirControl = 0.8f;
	GetCharacterMovement()->JumpZVelocity = 1000.0f;
	GetCharacterMovement()->GroundFriction = 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;
	// GetCharacterMovement()->bUseControllerDesiredRotation = false;
	
	zPosition = GetActorLocation().Z + 300.0f;
}

// Called when the game starts or when spawned
void ASurferCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(
		this, &ASurferCharacter::OnOverlapBegin);

	CanMove = true;
}

// Called every frame
void ASurferCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	tempPos = GetActorLocation();
	tempPos.X -= 850.0f;
	tempPos.Z = zPosition;
	SideViewCamera->SetWorldLocation(tempPos);
}

// Called to bind functionality to input
void ASurferCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveRight", this, &ASurferCharacter::MoveRight);
}

void ASurferCharacter::MoveRight(float Value)
{
	if (CanMove)
	{
		AddMovementInput(FVector(0.0f, 1.0f, 0.0f), Value);
	}
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
		
		AWallSpike* WallSpike = Cast<AWallSpike>(OtherActor);
		ASpike* Spike = Cast<ASpike>(OtherActor);

		if (WallSpike || Spike)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit a spike! Restarting..."));
			
			GetMesh()->Deactivate();
			GetMesh()->SetVisibility(false);

			CanMove = false;

			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(UnusedHandle,
				this, &ASurferCharacter::RestartLevel, 2.f, false);
		}
	}
}

