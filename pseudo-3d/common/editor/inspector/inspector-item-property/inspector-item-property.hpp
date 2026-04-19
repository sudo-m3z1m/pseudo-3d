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
	ARRAY,
	COLOR,
	BUTTON,
};

struct InspectorItemProperty
{
	PropertyType type;
	char property_name[32];
	
	union
	{
		void* property_ptr;
		void (*function_ptr)();
	};
};

#endif
