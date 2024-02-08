#include "gui.h"
int Gui::DrawType = 1;


Gui::Gui(Game game)
{
    // imgui initialization
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(game.window, true);
    ImGui_ImplOpenGL3_Init("#version 330");



}

Gui::~Gui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Gui::draw()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Debug", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove);
    
    if (ImGui::BeginTabBar("#tabs", ImGuiTabBarFlags_TabListPopupButton | ImGuiTabBarFlags_NoTooltip | ImGuiTabBarFlags_Reorderable))
    {
        if (ImGui::BeginTabItem("Delete"))
        {
            DrawType = 0;
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Grass")) 
        {
            DrawType = 1;
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Ground"))
        {
            DrawType = 2;
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
    ImGui::End();


    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}