#ifndef INSPECTOR_ITEM_PROPERTY_HPP
#define INSPECTOR_ITEM_PROPERTY_HPP

#include <stdio.h>
#include <stdint.h>

enum PropertyType : uint8_t
{
	INT,
	FLOAT,
	VECTOR2,
	WINDOW_COMPONENT,
	COLOR
};

struct InspectorItemProperty
{
	void* property_ptr;
	PropertyType type;
	char property_name[32];
};

#endif
