#ifndef INSPECTOR_ITEM_HPP
#define INSPECTOR_ITEM_HPP

#include <stdio.h>
#include <vector>

#include "inspector-item-property.hpp"

class InspectorItem
{
public:
	virtual ~InspectorItem() = default;
	virtual std::vector<InspectorItemProperty> get_inspector_item_properties() = 0;
	virtual const char* get_inspector_item_name() = 0;
};

#endif
