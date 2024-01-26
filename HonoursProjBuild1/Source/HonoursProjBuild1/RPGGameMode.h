// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CreatureBase.h"
#include "RPGGameMode.generated.h"

/**
 * 
 */
UCLASS()
class HONOURSPROJBUILD1_API ARPGGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	ARPGGameMode();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Function to add a new creature to the correct space on the initiative tracker
	UFUNCTION(BluePrintCallable)
	void AddCreature(UClass* CreatureToSpawn);

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BluePrintReadWrite)
	TArray<ACreatureBase*> initiativeOrder;

};
