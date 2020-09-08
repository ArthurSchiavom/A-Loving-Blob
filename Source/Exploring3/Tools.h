// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class EXPLORING3_API Tools
{
public:
	Tools();
	~Tools();

	/**
	Disables the given component and subcomponent's appearance. If chosen, collision will be disabled for UShapeComponents too.
	*/
	static void disableComponent(class UPrimitiveComponent* comp, bool disableCollision);
	static void enableComponent(class UPrimitiveComponent* comp, bool enableCollision);
};
