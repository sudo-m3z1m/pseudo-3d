#ifndef UPDATABLE_COMPONENT_H
#define UPDATABLE_COMPONENT_H

#include <stdio.h>

typedef struct UpdatableComponent
{
	void (*update)(float, void*);
} UpdatableComponent;

void update(float delta, void* owner);

#endif
