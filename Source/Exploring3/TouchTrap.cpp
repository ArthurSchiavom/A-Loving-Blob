// Fill out your copyright notice in the Description page of Project Settings.

#include "TouchTrap.h"
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"
#include "Exploring3Character.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

// Sets default values
ATouchTrap::ATouchTrap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	damageType = EDamageType::ONCE_PER_OVERLAP;
	damage = 1;
	canDamage = true;
	damageInterval = 1.0f;
}

// Called when the game starts or when spawned
void ATouchTrap::BeginPlay()
{
	Super::BeginPlay();

	overlapPrimitiveComponent = Cast<UPrimitiveComponent>(RootComponent);
	if (overlapPrimitiveComponent != nullptr) {
		overlapPrimitiveComponent->OnComponentBeginOverlap.AddDynamic(this, &ATouchTrap::onBeginOverlap);
		if (damageType == EDamageType::CONTINUOUS) {
			overlapPrimitiveComponent->OnComponentEndOverlap.AddDynamic(this, &ATouchTrap::onEndOverlap);
		}
	}
	else
		Destroy();
}

// Called every frame
void ATouchTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATouchTrap::setDamage(int32 newDamage)
{
	damage = newDamage;
}

void ATouchTrap::changeDamage(int32 damageVariation)
{
	damage = damage + damageVariation;
	if (damage < 0)
		damage = 0;
}

int32 ATouchTrap::getDamage()
{
	return int32();
}

void ATouchTrap::setCanDamage(bool newCanDamage)
{
	this->canDamage = newCanDamage;
}

bool ATouchTrap::getCanDamage()
{
	return canDamage;
}

float ATouchTrap::getDamageInterval()
{
	return damageInterval;
}

EDamageType ATouchTrap::getDamageType()
{
	return damageType;
}

void ATouchTrap::onBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResul)
{
	AExploring3Character* character = Cast<AExploring3Character>(OtherActor);
	if (character != nullptr) {
		switch (damageType) {
		case EDamageType::CONTINUOUS:
			GetWorldTimerManager().SetTimer(continuousDamageTimerHandle, this, &ATouchTrap::dealDamage, damageInterval, true, 0.0f);
			break;
		case EDamageType::ONCE_PER_OVERLAP:
			dealDamage();
			break;
		case EDamageType::ONE_TIME_ONLY:
			dealDamage();
			canDamage = false;
			break;
		case EDamageType::ONE_TIME_ONLY_DESTROY:
			dealDamage();
			Destroy();
			break;
		}
	}
}

void ATouchTrap::onEndOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	AExploring3Character* character = Cast<AExploring3Character>(OtherActor);
	if (character != nullptr) {
		if (damageType == EDamageType::CONTINUOUS) {
			GetWorldTimerManager().ClearTimer(continuousDamageTimerHandle);
		}
	}
}

void ATouchTrap::dealDamage() {
	Cast<AExploring3Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->takeDamage(damage);
}