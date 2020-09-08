// Fill out your copyright notice in the Description page of Project Settings.

#include "Tools.h"
#include "Components/SceneComponent.h"
#include "Components/ShapeComponent.h"

Tools::Tools()
{
}

Tools::~Tools()
{
}

void Tools::disableComponent(class UPrimitiveComponent* comp, bool disableCollision)
{
	if (comp == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("nullptr UPrimitiveComponent"));
		return;
	}

	TArray<USceneComponent*> children;
	comp->GetChildrenComponents(true, children);

	comp->SetVisibility(false, true);
	if (disableCollision) {
		comp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	for (USceneComponent* child : children) {
		UShapeComponent* shapeChild = Cast<UShapeComponent>(child);
		if (shapeChild != nullptr) {
			shapeChild->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

void Tools::enableComponent(class UPrimitiveComponent* comp, bool enableCollision)
{
	TArray<USceneComponent*> children;
	comp->GetChildrenComponents(true, children);

	comp->SetVisibility(true, true);

	if (enableCollision) {
		comp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	for (USceneComponent* child : children) {
		if (enableCollision) {
			UShapeComponent* shapeChild = Cast<UShapeComponent>(child);
			if (shapeChild != nullptr) {
				shapeChild->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			}
		}
	}
}