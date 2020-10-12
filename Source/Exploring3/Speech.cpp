// Fill out your copyright notice in the Description page of Project Settings.


#include "Speech.h"

// Sets default values
ASpeech::ASpeech()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpeech::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->;
}

// Called every frame
void ASpeech::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

