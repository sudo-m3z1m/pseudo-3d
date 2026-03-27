#ifndef RENDERER_COLUMN_HPP
#define RENDERER_COLUMN_HPP

#include <vector>

#include "constants.hpp"

class RendererColumn
{
public:
	int bottom, top;
	int u, v_top, v_bottom;
	
	RendererColumn();
	RendererColumn(int bottom, int top);
	RendererColumn(int bottom, int top, int u, int v_top, int v_bottom);
	
	std::vector<RendererColumn> subtract_columns(std::vector<RendererColumn> columns);
	std::vector<RendererColumn> merge_columns(std::vector<RendererColumn> columns);
};

#endif
