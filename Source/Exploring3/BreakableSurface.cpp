// Fill out your copyright notice in the Description page of Project Settings.

#include "BreakableSurface.h"
#include "Paper2D/Classes/PaperFlipbookComponent.h"
#include "Paper2D/Classes/PaperSpriteComponent.h"
#include "Paper2D/Classes/PaperFlipbook.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "Components/CapsuleComponent.h"
#include "Exploring3Character.h"
#include "Tools.h"

// Sets default values
ABreakableSurface::ABreakableSurface()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Hp = 0;
	Destroyed = false;
	AppearanceType = EAppearanceType::SPRITE;

	DummyRootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy root component"));
	RootComponent = DummyRootComp;

	FlipbookAppearance = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Paper Flipbook"));
	SpriteAppearance = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Paper Sprite"));
	DestroyedFlipbookAppearance = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Destroyed Paper Flipbook"));
	DestroyedSpriteAppearance = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Destroyed Paper Sprite"));
	DestructionAnimationFlipbookAppearance = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Destr Paper Flipbook"));


	FlipbookAppearance->SetupAttachment(RootComponent);
	SpriteAppearance->SetupAttachment(RootComponent);
	DestroyedFlipbookAppearance->SetupAttachment(RootComponent);
	DestroyedSpriteAppearance->SetupAttachment(RootComponent);
	DestructionAnimationFlipbookAppearance->SetupAttachment(RootComponent);

	OverlapCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Overlap capsule"));
	OverlapCapsule->InitCapsuleSize(10, 10);
	OverlapCapsule->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABreakableSurface::BeginPlay()
{
	Super::BeginPlay();

	if (Flipbooks.Num() > 0) {
		AppearanceType = EAppearanceType::FLIPBOOK;
		Hp = Flipbooks.Num();
	}
	else if (Sprites.Num() > 0) {
		AppearanceType = EAppearanceType::SPRITE;
		Hp = Sprites.Num();
		SpriteAppearance->SetMobility(EComponentMobility::Stationary);
	}
	else {
		Destroy();
		return;
	}

	if (DestructionAnimationFlipbook != nullptr) {
		DestructionAnimationFlipbookAppearance->SetFlipbook(DestructionAnimationFlipbook);
		DestructionAnimationFlipbookAppearance->SetLooping(false);
		DestructionAnimationFlipbookAppearance->OnFinishedPlaying.AddDynamic(this, &ABreakableSurface::SetFinalAppearance);
		DestructionAnimationFlipbookAppearance->Stop();
		HasDestructionAnimation = true;
		Tools::disableComponent(Cast<UPrimitiveComponent>(DestructionAnimationFlipbookAppearance), true);
	}
	else {
		HasDestructionAnimation = false;
	}

	if (DestroyedFlipbook != nullptr) {
		DestroyedFlipbookAppearance->SetFlipbook(DestroyedFlipbook);
		DestroyedAppearanceType = EDestroyedAppearanceType::FLIPBOOK;
		Tools::disableComponent(Cast<UPrimitiveComponent>(DestroyedFlipbookAppearance), true);
	}
	else if (DestroyedSprite != nullptr) {
		DestroyedSpriteAppearance->SetSprite(DestroyedSprite);
		DestroyedAppearanceType = EDestroyedAppearanceType::SPRITE;
		Tools::disableComponent(Cast<UPrimitiveComponent>(DestroyedSpriteAppearance), true);
	}
	else {
		DestroyedAppearanceType = EDestroyedAppearanceType::NONE;
	}

	UpdateAppearance();
}

void ABreakableSurface::UpdateAppearance()
{
	if (Hp < 1)
		return;

	switch (AppearanceType) {
	case EAppearanceType::FLIPBOOK:
		FlipbookAppearance->SetFlipbook(Flipbooks[Hp - 1]);
		break;
	case EAppearanceType::SPRITE:
		SpriteAppearance->SetSprite(Sprites[Hp - 1]);
	}
}

// Called every frame
void ABreakableSurface::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABreakableSurface::takeDamage(int32 damage)
{
	if (Hp == 0)
		return;

	Hp = Hp - damage;
	if (Hp < 0)
		Hp = 0;

	if (Hp == 0) {
		Tools::disableComponent(Cast<UPrimitiveComponent>(FlipbookAppearance), true);
		Tools::disableComponent(Cast<UPrimitiveComponent>(SpriteAppearance), true);
		BreakSurface();
	}

	UpdateAppearance();
}

void ABreakableSurface::SetFinalAppearance()
{
	if (HasDestructionAnimation) {
		Tools::disableComponent(Cast<UPrimitiveComponent>(DestructionAnimationFlipbookAppearance), true);
	}

	switch (DestroyedAppearanceType) {
	case EDestroyedAppearanceType::FLIPBOOK:
		Tools::enableComponent(Cast<UPrimitiveComponent>(DestroyedFlipbookAppearance), true);
		break;
	case EDestroyedAppearanceType::SPRITE:
		Tools::enableComponent(Cast<UPrimitiveComponent>(DestroyedSpriteAppearance), true);
	}
}

void ABreakableSurface::BreakSurface()
{
	if (HasDestructionAnimation) {
		Tools::enableComponent(Cast<UPrimitiveComponent>(DestructionAnimationFlipbookAppearance), true);
		DestructionAnimationFlipbookAppearance->PlayFromStart();
	}
	else {
		SetFinalAppearance();
	}
}