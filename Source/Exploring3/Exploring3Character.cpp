// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Exploring3Character.h"
#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "TouchTrap.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"
#include "Paper2D/Classes/PaperFlipbookComponent.h"
#include "BreakableSurface.h"

DEFINE_LOG_CATEGORY_STATIC(SideScrollerCharacter, Log, All);

//////////////////////////////////////////////////////////////////////////
// AExploring3Character


AExploring3Character::AExploring3Character()
{
	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
	GetCapsuleComponent()->SetCapsuleRadius(40.0f);

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	// Create an orthographic camera (no perspective) and attach it to the boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	SideViewCameraComponent->OrthoWidth = 2048.0f;
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Prevent all automatic rotation behavior on the camera, character, and camera component
	CameraBoom->SetUsingAbsoluteRotation(true);
	SideViewCameraComponent->bUsePawnControlRotation = false;
	SideViewCameraComponent->bAutoActivate = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Configure character movement
	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;

	// Lock character motion onto the XZ plane, so the character can't move in or out of the screen
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	// Behave like a traditional 2D platformer character, with a flat bottom instead of a curved capsule bottom
	// Note: This can cause a little floating when going up inclines; you can choose the tradeoff between better
	// behavior on the edge of a ledge versus inclines by setting this to true or false
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;

    // 	TextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarGear"));
    // 	TextComponent->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
    // 	TextComponent->SetRelativeLocation(FVector(35.0f, 5.0f, 20.0f));
    // 	TextComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
    // 	TextComponent->SetupAttachment(RootComponent);

	// Enable replication on the Sprite component so animations show up when networked
	GetSprite()->SetIsReplicated(true);
	bReplicates = true;

	maxHp = 200;

	overlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Overlap box"));
	overlapBox->InitBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	overlapBox->SetCollisionProfileName("OverlapIdentifier");
	overlapBox->SetupAttachment(RootComponent);

	isAttackingSurface = false;
}

int32 AExploring3Character::getMaxHp()
{
	return maxHp;
}

int32 AExploring3Character::getHp()
{
	return hp;
}

void AExploring3Character::setHp(int32 newHp)
{
	hp = newHp;
}

void AExploring3Character::takeDamage(int32 damagee)
{
	hp = hp - damagee;
	if (hp < 0)
		hp = 0;
}

//////////////////////////////////////////////////////////////////////////
// Animation

void AExploring3Character::UpdateAnimation()
{
	/*
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();

	// Are we moving or standing still?
	UPaperFlipbook* DesiredAnimation = (PlayerSpeedSqr > 0.0f) ? RunningAnimation : IdleAnimation;
	if( GetSprite()->GetFlipbook() != DesiredAnimation 	)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
	}
	*/
}

void AExploring3Character::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	UpdateCharacter();	
}


//////////////////////////////////////////////////////////////////////////
// Input

void AExploring3Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Note: the 'Jump' action and the 'MoveRight' axis are bound to actual keys/buttons/sticks in DefaultInput.ini (editable from Project Settings..Input)
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AExploring3Character::jumpp);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AExploring3Character::stopJumpingg);
	PlayerInputComponent->BindAxis("MoveRight", this, &AExploring3Character::MoveRight);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &AExploring3Character::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AExploring3Character::TouchStopped);

	PlayerInputComponent->BindAction("AttackFloor", IE_Pressed, this, &AExploring3Character::surfaceAttack);
}

void AExploring3Character::MoveRight(float Value)
{
	/*UpdateChar();*/

	// Apply the input to the character motion
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
}

void AExploring3Character::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// Jump on any touch
	Jump();
}

void AExploring3Character::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// Cease jumping once touch stopped
	StopJumping();
}

void AExploring3Character::UpdateCharacter()
{
	// Update animation to match the motion
	UpdateAnimation();

	// Now setup the rotation of the controller based on the direction we are travelling
	const FVector PlayerVelocity = GetVelocity();	
	float TravelDirection = PlayerVelocity.X;
	// Set the rotation so that the character faces his direction of travel.
	if (Controller != nullptr)
	{
		if (TravelDirection < 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
		}
		else if (TravelDirection > 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
		}
	}
}

void AExploring3Character::BeginPlay()
{
	Super::BeginPlay();

	hp = maxHp;
	GetSprite()->OnFinishedPlaying.AddDynamic(this, &AExploring3Character::finishedPlayingAnim);
}

void AExploring3Character::surfaceAttack()
{
	GetSprite()->SetFlipbook(surfaceAttackAnim);
	GetSprite()->SetLooping(false);
	GetSprite()->PlayFromStart();
	isAttackingSurface = true;
}

void AExploring3Character::jumpp()
{
	if (!isAttackingSurface)
		ACharacter::Jump();
}

void AExploring3Character::stopJumpingg()
{
	ACharacter::StopJumping();
}

void AExploring3Character::finishedPlayingAnim()
{
	TArray<AActor*> actors;
	GetOverlappingActors(actors);
	UPaperFlipbook* currentFlipbook = GetSprite()->GetFlipbook();

	if (currentFlipbook == surfaceAttackAnim) {
		ABreakableSurface* breakableSurface;
		for (AActor* actor : actors) {
			breakableSurface = Cast<ABreakableSurface>(actor);
			if (breakableSurface != nullptr) {
				breakableSurface->takeDamage(1);
			}
		}
	}

	GetSprite()->SetFlipbook(IdleAnimation);
	GetSprite()->SetLooping(true);
	GetSprite()->PlayFromStart();
	isAttackingSurface = false;
}