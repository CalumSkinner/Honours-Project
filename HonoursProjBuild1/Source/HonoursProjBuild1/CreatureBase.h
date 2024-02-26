// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataStructs.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "CreatureBase.generated.h"

UCLASS()
class HONOURSPROJBUILD1_API ACreatureBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACreatureBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Functions

	// Function to update status effect durations
	UFUNCTION()
	void UpdateEffects();

	// Function to play a given sound cue, used as timer delegate for delayed sound function
	UFUNCTION()
	void PlaySound(USoundCue* SoundCue);

	// Function to play a given sound cue after a specified delay in seconds
	UFUNCTION()
	void PlaySoundWithDelay(USoundCue* SoundCue, float Delay);

	// Variables

	// StatSheet struct to hold characters core stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FStatSheet BaseStats;

	// Current health value
	UPROPERTY()
	int HealthCurrent = 30;

	// Value to store initiative for current combat
	UPROPERTY()
	int InitiativeRoll = 0;

	// List of available moves
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMove> MoveList;

	// Map holding status effects currently applied to this creature and their durations
	UPROPERTY()
	TMap<EStatusEffect, int> CurrentEffects;

	// Sound cue to play when activated
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* ReadySound;

	// Sound cue for when low on HP
	UPROPERTY()
	USoundCue* LowHPSound;

	// Sound cue for when a move misses
	UPROPERTY()
	USoundCue* MissSound;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Functions

	// Function to update creature at the start of a new turn
	UFUNCTION()
	void TurnStart();

	// Function to use a selected move, returns a float value of approximate time taken in seconds
	UFUNCTION()
	float UseMove(FMove Move, TArray<ACreatureBase*> Targets);

	// Function to play the appropriate activation sound based on current health
	UFUNCTION()
	void PlayReadySound();

	// Function to apply a status effect to this creature
	UFUNCTION()
	void AddEffect(EStatusEffect Effect, int Duration);

	// Function to return the initiative roll for this creature
	UFUNCTION()
	int GetInitiativeRoll();

	// Function to return the stat sheet for this creature
	UFUNCTION(BluePrintCallable)
	FStatSheet GetStats();

	// Function to return move list
	UFUNCTION()
	TArray<FMove> GetMoveList();

	// Function to return current health value
	UFUNCTION(BluePrintCallable)
	int GetHealth();

	// Function to set health to a given amount
	UFUNCTION()
	void SetHealth(int Value);

	// Variables

	// Sound cue to play when damaged
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* DamagedSound;

	// Sound cue to play when killed
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* DeathSound;

};