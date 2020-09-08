// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BreakableSurface.generated.h"

UENUM(BlueprintType)
enum class EDestroyedAppearanceType : uint8 {
	SPRITE UMETA(DisplayName = "SPRITE"),
	FLIPBOOK UMETA(DisplayName = "FLIPBOOK"),
	NONE UMETA(DisplayName = "NONE")
};

UENUM(BlueprintType)
enum class EAppearanceType : uint8 {
	SPRITE UMETA(DisplayName = "SPRITE"),
	FLIPBOOK UMETA(DisplayName = "FLIPBOOK")
};

UCLASS()
class EXPLORING3_API ABreakableSurface : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABreakableSurface();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Organization)
		class USceneComponent* DummyRootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Appearance)
		class UPaperFlipbookComponent* FlipbookAppearance;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Appearance)
		class UPaperSpriteComponent* SpriteAppearance;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Appearance)
		class UPaperFlipbookComponent* DestroyedFlipbookAppearance;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Appearance)
		class UPaperSpriteComponent* DestroyedSpriteAppearance;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Appearance)
		class UPaperFlipbookComponent* DestructionAnimationFlipbookAppearance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health)
		int Hp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health)
		bool Destroyed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AppearanceConfig)
		TArray<class UPaperFlipbook*> Flipbooks;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AppearanceConfig)
		TArray<class UPaperSprite*> Sprites;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AppearanceConfig)
		class UPaperFlipbook* DestructionAnimationFlipbook;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AppearanceConfig)
		class UPaperFlipbook* DestroyedFlipbook;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AppearanceConfig)
		class UPaperSprite* DestroyedSprite;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Appearance)
		EAppearanceType AppearanceType;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Appearance)
		EDestroyedAppearanceType DestroyedAppearanceType;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Appearance)
		bool HasDestructionAnimation;

	UFUNCTION()
		void UpdateAppearance();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Appearance)
		class UCapsuleComponent* OverlapCapsule;

	UFUNCTION()
		void SetFinalAppearance();
	UFUNCTION()
		void BreakSurface();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void takeDamage(int32 damage);
};