#include "inspector.hpp"

Inspector::Inspector()
{
	
}

Inspector::Inspector(InspectorItem*& item, int width, int height, Vector2D<float> viewport_size)
{
	current_item = item;
	this->width = width;
	this->height = height;
	
	screen_position.x = viewport_size.x - width;
	screen_position.y = 16;
}

Inspector::~Inspector()
{
	
}

void Inspector::render_inspector()
{
	if(!current_item) return;
	
	std::vector<InspectorItemProperty> item_properties = current_item->get_inspector_item_properties();
	ImVec2 window_pos = {screen_position.x, screen_position.y};
	ImGui::SetNextWindowPos(window_pos);
	ImGui::SetNextWindowSize({width, height});
	
	ImGui::Begin("Inspector menu", NULL, window_flags);
	ImGui::Text(current_item->get_inspector_item_name(), 0);
	ImGui::Separator();
	
	for(InspectorItemProperty& property : item_properties)
	{
		render_item_property(property);
	}
	
	ImGui::End();
}

void Inspector::render_item_property(InspectorItemProperty& property)
{
	switch (property.type)
	{
		case INT:
			render_int_property(property);
			break;
		case FLOAT:
			render_float_property(property);
			break;
		case VECTOR2:
			render_vector_property(property);
			break;
		case WINDOW_COMPONENT:
			render_window_component_property(property);
			break;
		case COLOR:
			render_color_property(property);
			break;
		case BUTTON:
			render_button_property(property);
			break;
		case ARRAY:
			render_array_property(property);
			break;
		default:
			break;
	}
}

void Inspector::render_int_property(InspectorItemProperty& property)
{
	int* int_ptr = static_cast<int*>(property.property_ptr);
	ImGui::InputInt(property.property_name, int_ptr);
}

void Inspector::render_float_property(InspectorItemProperty& property)
{
	float* float_ptr = static_cast<float*>(property.property_ptr);
	ImGui::DragFloat(property.property_name, float_ptr, 0.01);
}

void Inspector::render_vector_property(InspectorItemProperty& property)
{
	Vector2D<float>* vector_ptr = static_cast<Vector2D<float>*>(property.property_ptr);
	
	ImGui::Text(property.property_name, 0);
	ImGui::DragFloat("x", &vector_ptr->x, 0.01);
	ImGui::DragFloat("y", &vector_ptr->y, 0.01);
}

void Inspector::render_window_component_property(InspectorItemProperty& property)
{
	WindowComponent** window_component_ptr = static_cast<WindowComponent**>(property.property_ptr);
	
	ImGui::Text("Window component: %p", property.property_ptr);
	ImGui::InputInt("Front sector id", &(*window_component_ptr)->f_sector_index);
	ImGui::InputInt("Back point id", &(*window_component_ptr)->s_sector_index);
	ImGui::Text("Window textures ids");
	ImGui::InputInt("Bottom texture index", &(*window_component_ptr)->bottom_tid);
	ImGui::InputInt("Upper texture index", &(*window_component_ptr)->upper_tid);
	
	if (ImGui::Button("Remove window component")) *window_component_ptr = nullptr;
}

void Inspector::render_array_property(InspectorItemProperty& property)
{
	if(!ImGui::CollapsingHeader(property.property_name))
		return;

	std::vector<InspectorItemProperty> array = *(static_cast<std::vector<InspectorItemProperty>*>(property.property_ptr));
	for(int property_index = 0; property_index < array.size(); property_index++)
	{
		ImGui::PushID(property_index);
		render_item_property(array[property_index]);
		ImGui::PopID();
	}
}

void Inspector::render_color_property(InspectorItemProperty& property)
{
	
}

void Inspector::render_button_property(InspectorItemProperty& property)
{
	if(ImGui::Button(property.property_name)) property.function_ptr(current_item);
}

