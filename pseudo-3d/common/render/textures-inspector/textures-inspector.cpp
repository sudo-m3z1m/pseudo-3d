#include "textures-inspector.hpp"

TexturesInspector::TexturesInspector()
{
	this->width = 0;
	this->height = 0;
	screen_position = {0, 0};
}

TexturesInspector::TexturesInspector(int width, int height, Vector2D<float> viewport_size)
{
	this->width = width;
	this->height = height;
	screen_position = {viewport_size.x - width, viewport_size.y - height};
}

TexturesInspector::~TexturesInspector() { }

void TexturesInspector::render(TextureBuffer*& buffer)
{
	std::vector<std::string> textures_names = buffer->get_textures_names();
	
	ImGui::SetNextWindowPos({screen_position.x, screen_position.y});
	ImGui::SetNextWindowSize({width, height});
	
	ImGui::Begin("textures", NULL, window_flags);
	ImGui::Text("Textures");
	ImGui::Separator();
	
	ImGui::BeginListBox("##Textures", {-FLT_MIN, -FLT_MIN});
	for(int texture_index = 0; texture_index < textures_names.size(); texture_index++)
	{
		ImGui::PushID(texture_index);
		ImGui::Text(std::to_string(texture_index).c_str(), 0);
		ImGui::SameLine();
		ImGui::Text(textures_names[texture_index].c_str(), 0);
		ImGui::PopID();
	}
	ImGui::EndListBox();
	
	ImGui::End();
}
