#include "editor-renderer.hpp"

EditorRenderer::EditorRenderer() : Renderer()
{
	
}

EditorRenderer::EditorRenderer(Camera* camera, TextureBuffer* texture_buffer, int width, int height) :
	Renderer(camera, texture_buffer, width, height)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL3_InitForSDLRenderer(application_window, application_renderer);
	ImGui_ImplSDLRenderer3_Init(application_renderer);
}

void EditorRenderer::render()
{
	SDL_RenderClear(application_renderer);
	
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
	
	ImGui::Begin("Tools");
	if (ImGui::Button("Save Map")) { /* ... */ }
	ImGui::End();
	
	ImGui::Render();
	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), application_renderer);
	SDL_RenderPresent(application_renderer);
}
