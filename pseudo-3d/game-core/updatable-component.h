#ifndef UPDATABLE_COMPONENT_H
#define UPDATABLE_COMPONENT_H

#include <stdio.h>

typedef struct UpdatableComponent
{
	void (*update)(float);
} UpdatableComponent;

void update(float delta);

#endif
