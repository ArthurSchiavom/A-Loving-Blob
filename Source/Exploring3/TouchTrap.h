// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TouchTrap.generated.h"

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	CONTINUOUS UMETA(DisplayName = "CONTINUOUS"),
	ONCE_PER_OVERLAP UMETA(DisplayName = "ONCE_PER_OVERLAP"),
	ONE_TIME_ONLY UMETA(DisplayName = "ONE_TIME_ONLY"),
	ONE_TIME_ONLY_DESTROY UMETA(DisplayName = "ONE_TIME_ONLY_DESTROY")
};

UCLASS()
class EXPLORING3_API ATouchTrap : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATouchTrap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		int32 damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		bool canDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		float damageInterval;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		EDamageType damageType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Overlap)
		UPrimitiveComponent* overlapPrimitiveComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Timer)
		FTimerHandle continuousDamageTimerHandle;

	UFUNCTION()
		void dealDamage();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void setDamage(int32 newDamage);
	UFUNCTION()
		void changeDamage(int32 damageVariation);
	UFUNCTION()
		int32 getDamage();

	UFUNCTION()
		void setCanDamage(bool newCanDamage);
	UFUNCTION()
		bool getCanDamage();

	UFUNCTION()
		float getDamageInterval();

	UFUNCTION()
		EDamageType getDamageType();

	UFUNCTION()
		void onBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResul);

	UFUNCTION()
		void onEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
