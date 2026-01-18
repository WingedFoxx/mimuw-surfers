#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SurferSaveGame.generated.h"

UCLASS()
class MIMUWSURFERS_API USurferSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	// The variable we want to save
	UPROPERTY(VisibleAnywhere, Category = Basic)
	float HighScore;

	USurferSaveGame()
	{
		HighScore = 0.0f;
	}
};