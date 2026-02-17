#ifndef RENDERER_COLUMN_HPP
#define RENDERER_COLUMN_HPP

#include <vector>

#include "constants.hpp"

class RendererColumn
{
public:
	int bottom, top;
	
	RendererColumn();
	RendererColumn(int bottom, int top);
	
	std::vector<RendererColumn> subtract_columns(std::vector<RendererColumn> columns);
	std::vector<RendererColumn> merge_columns(std::vector<RendererColumn> columns);
};

#endif
