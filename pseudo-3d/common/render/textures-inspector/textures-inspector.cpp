#include "textures-inspector.hpp"

TexturesInspector::TexturesInspector()
{
	this->width = 0;
	this->height = 0;
	screen_position = {0, 0};
}

TexturesInspector::TexturesInspector(int width, int height, Vector2D<float>& viewport_size)
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
	for(int tid = 0; tid < textures_names.size(); tid++)
	{
		std::filesystem::path path = textures_names[tid];
		ImGui::PushID(tid);
		ImGui::Text(std::to_string(tid).c_str(), 0);
		
		ImGui::SameLine();
		ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
		ImGui::SameLine();
		
		ImGui::Text(path.filename().string().c_str(), 0);
		
		ImGui::SameLine();

		if(ImGui::Button("Remove")) buffer->remove_texture(tid);
		ImGui::PopID();
	}
	ImGui::EndListBox();
	
	ImGui::End();
}
