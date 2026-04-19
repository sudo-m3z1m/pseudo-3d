#include "inspector.hpp"

Inspector::Inspector()
{
	
}

Inspector::Inspector(InspectorItem* item)
{
	
}

Inspector::~Inspector()
{
	
}

void Inspector::render_inspector()
{
	std::vector<InspectorItemProperty> item_properties = current_item->get_inspector_item_properties();
	ImVec2 window_pos = {screen_position.x, screen_position.y};
//	window_pos.y = 16;
//	window_pos.x -= 140;
	ImGui::SetNextWindowPos(window_pos);
//	ImGui::SetNextWindowSize({140, 400});
	ImGui::SetNextWindowSize({width, height});
	
//	ImGui::Begin("Inspector menu", NULL, window_flags);
	ImGui::Text(current_item->get_inspector_item_name(), 0);
	ImGui::Separator();
	
	for(InspectorItemProperty& property : item_properties)
	{
		
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
	ImGui::InputFloat(property.property_name, float_ptr);
}

void Inspector::render_vector_property(InspectorItemProperty& property)
{
	Vector2D<float>* vector_ptr = static_cast<Vector2D<float>*>(property.property_ptr);
	
	ImGui::Text(property.property_name, 0);
	ImGui::InputFloat("x", &vector_ptr->x);
	ImGui::InputFloat("y", &vector_ptr->y);
}

void Inspector::render_window_component_property(InspectorItemProperty& property)
{
	WindowComponent** window_component_ptr = static_cast<WindowComponent**>(property.property_ptr);
	
	ImGui::Text("Window component: %p", property.property_ptr);
	ImGui::InputInt("Front sector id", &(*window_component_ptr)->f_sector_index);
	ImGui::InputInt("Back point id", &(*window_component_ptr)->s_sector_index);
	ImGui::Text("Window textures ids");
	
	if (ImGui::Button("Remove window component")) *window_component_ptr = nullptr;
}

void Inspector::render_array_property(InspectorItemProperty& property)
{
	std::vector<InspectorItemProperty>* array_ptr = static_cast<std::vector<InspectorItemProperty>*>(property.property_ptr);
	for (int array_index = 0; array_index < array_ptr->size(); array_index++)
	{
		
	}
//	ImGui::Text(property.property_name, 0);
//	ImGui::BeginListBox("Walls");
//	ImGui::EndListBox();
}

void Inspector::render_color_property(InspectorItemProperty& property)
{
	
}

void Inspector::render_button_property(InspectorItemProperty& property)
{
	if(ImGui::Button(property.property_name)) property.function_ptr();
}

