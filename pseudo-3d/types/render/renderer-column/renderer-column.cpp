#include "renderer-column.hpp"

RendererColumn::RendererColumn()
{
	bottom = DEFAULT_SCREEN_HEIGHT;
	top = 0;
}

RendererColumn::RendererColumn(int bottom, int top)
{
	this->bottom = bottom;
	this->top = top;
}

RendererColumn::RendererColumn(int bottom, int top, int u, float v_top, float v_step)
{
	this->bottom = bottom;
	this->top = top;
	
	this->u = u;
	this->v_top = v_top;
	this->v_step = v_step;
}

std::vector<RendererColumn> RendererColumn::subtract_columns(std::vector<RendererColumn> columns)
{
	std::vector<RendererColumn> ranges;
	ranges.push_back(*(this));
	for(RendererColumn current_column : columns)
	{
		std::vector<RendererColumn> new_ranges;
		for(RendererColumn range : ranges)
		{
			if((range.bottom < current_column.top) || (range.top > current_column.bottom))
			{
				new_ranges.push_back(range);
				continue;
			}
			if(range.top < current_column.top) new_ranges.push_back(RendererColumn(current_column.top, range.top));
			if(range.bottom > current_column.bottom) new_ranges.push_back(RendererColumn(range.bottom, current_column.bottom));
		}
		
		if(new_ranges.size() == 0) return std::vector<RendererColumn>();
		ranges = new_ranges; //Probably isn't working
	}
	
	return ranges;
}

std::vector<RendererColumn> RendererColumn::merge_columns(std::vector<RendererColumn> columns)
{
	std::vector<RendererColumn> new_renderer_columns;
	RendererColumn new_column = *this;
	
	for(RendererColumn current_column : columns)
	{
		if((new_column.bottom < current_column.top) || (new_column.top > current_column.bottom))
		{
			new_renderer_columns.push_back(current_column);
			continue;
		}
		new_column.bottom = std::max(new_column.bottom, current_column.bottom);
		new_column.top = std::min(new_column.top, current_column.top);
	}
	
	new_renderer_columns.push_back(new_column);
	return new_renderer_columns;
}
