// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Exploring3GameMode.h"
#include "Exploring3Character.h"

AExploring3GameMode::AExploring3GameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = AExploring3Character::StaticClass();	
}
