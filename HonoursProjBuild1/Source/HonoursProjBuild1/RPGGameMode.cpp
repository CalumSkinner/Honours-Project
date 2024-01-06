// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGGameMode.h"

ARPGGameMode::ARPGGameMode() {

	PrimaryActorTick.bCanEverTick = true;

}

void ARPGGameMode::BeginPlay()
{

	Super::BeginPlay();

}

void ARPGGameMode::Tick(float DeltaTime)
{



}

// Function to add a new creature to the correct space on the initiative tracker
void ARPGGameMode::AddCreature(ACreatureBase* creature) {

	// If tracker is empty add to the first entry
	if (initiativeOrder.IsEmpty()) {

		initiativeOrder.Add(creature);

	}
	else {

		bool added = false;

		// Check initiative roll against the current array entries and add when initiative roll is higher than the current entry
		for (int i = 0; i < initiativeOrder.Num(); i++) {

			if (creature->GetInitiativeRoll() > initiativeOrder[i]->GetInitiativeRoll()) {

				initiativeOrder.Insert(creature, i);
				added = true;
				break;

			}
			else {

				i++;

			}

		}

		// If the end of the array is reached and entry has still not been added, add to the end of the array
		if (!added) {

			initiativeOrder.Add(creature);

		}

	}

}