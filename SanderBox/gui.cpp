#include "gui.h"
#include "game.h"

#include <filesystem>


// Variables -----------------------------------
int Gui::DrawType = 1;
bool Gui::UseSim = true;
bool Gui::Ortho = false;
std::vector<std::vector<unsigned int>>* gameMap;

char Gui::fileName[128] = "save1";
const char* SavingOutput = "";
const char* LoadingOutput = "";

float Gui::SimSpeed = 1;

static std::string current_item = "Defaultsave";

Gui::Gui(Game* game)
{
    // imgui initialization
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(game->window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Gets map pointer for saving
    gameMap = &game->Map;
}

Gui::~Gui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

const char* selected = "Grass";
bool isDelete = false;
int oldDrawType;
ImVec4 color = ImVec4(0.6, 0.8f, 0.0f, 1.f);
ImVec4 deleteColor = (ImVec4)ImColor::HSV(1, 0.2f, 0.6f);
void Gui::draw()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //Begins window
    ImGui::Begin("Debug", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
    

    //Helps find gui examples
    //ImGui::ShowDemoWindow((bool*)1);
    

    ImGui::PushStyleColor(ImGuiCol_Button, deleteColor);
    ImGui::Spacing();
    ImGui::Text(""); ImGui::SameLine();

    //Creates delete button
    if (ImGui::Button("Delete", ImVec2(100, 20)))
    {
        // Checks if delete is selected and saves the element used before
        isDelete = !isDelete;
        if (isDelete)
        {
            oldDrawType = DrawType;
            deleteColor = (ImVec4)ImColor::HSV(0, 0.6f, 0.6f);
            DrawType = 0;
        }
        if (!isDelete)
        {
            DrawType = oldDrawType;
            deleteColor = (ImVec4)ImColor::HSV(1, 0.2f, 0.6f);
        }
    }

    // Checks if you have selected a new element
    if (DrawType != 0)
        isDelete = false;

    ImGui::SameLine();
    ImGui::PopStyleColor(1);


    ImGui::PushStyleColor(ImGuiCol_Button,        color);
    // Opens element popup
    if (ImGui::Button(selected, ImVec2(100, 20)))
        ImGui::OpenPopup("my_file_popup");
    ImGui::PopStyleColor(1);

    // Popup window for elements
    if (ImGui::BeginPopup("my_file_popup", ImGuiWindowFlags_MenuBar))
    {
        ImGui::Text("Select material");

        // Grass -------------------------------------------------------------
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6, 0.8f, 0.0f, 1.f));
        if (ImGui::Button("Grass")) {
            ImGui::CloseCurrentPopup();
            // Changes selection button color and text
            color = ImVec4(0.6, 0.8f, 0.0f, 1.f);
            selected = "Grass";
            DrawType = 1;

            // changes delete button color to grey
            deleteColor = (ImVec4)ImColor::HSV(1, 0.2f, 0.6f);
        }
        ImGui::PopStyleColor(1);
        ImGui::SameLine();

        // Ground -------------------------------------------------------------
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.4f, 0.1f, 1.f));
        if (ImGui::Button("Ground")) {
            ImGui::CloseCurrentPopup();
            color = ImVec4(0.7f, 0.4f, 0.1f, 1.f);
            selected = "Ground";
            DrawType = 2;

            deleteColor = (ImVec4)ImColor::HSV(1, 0.2f, 0.6f);
        }
        ImGui::PopStyleColor(1);
        ImGui::SameLine();

        // Sand -------------------------------------------------------------
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.922f, 0.847f, 0.437f, 1.f));
        if (ImGui::Button("Sand")) {
            ImGui::CloseCurrentPopup();
            color = ImVec4(0.922f, 0.847f, 0.437f, 1.f);
            selected = "Sand";
            DrawType = 3;

            deleteColor = (ImVec4)ImColor::HSV(1, 0.2f, 0.6f);
        }
        ImGui::PopStyleColor(1);

        // Rock -------------------------------------------------------------
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.459f, 0.447f, 0.447f, 1.f));
        if (ImGui::Button("Rock")) {
            ImGui::CloseCurrentPopup();
            color = ImVec4(0.459f, 0.447f, 0.447f, 1.f);
            selected = "Rock";
            DrawType = 4;

            deleteColor = (ImVec4)ImColor::HSV(1, 0.2f, 0.6f);
        }
        ImGui::PopStyleColor(1);
        ImGui::SameLine();

        // Water -------------------------------------------------------------
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.612f, 0.678f, 0.98f, 1.f));
        if (ImGui::Button("Water")) {
            ImGui::CloseCurrentPopup();
            color = ImVec4(0.612f, 0.678f, 0.98f, 1.f);
            selected = "Water";
            DrawType = 5;

            deleteColor = (ImVec4)ImColor::HSV(1, 0.2f, 0.6f);
        }
        ImGui::PopStyleColor(1);
        ImGui::EndPopup();
    }
    ImGui::SameLine();
    //creates a gap between buttons
    ImGui::Text("                                          ");
    ImGui::SameLine();

    //Opens settings window
    if (ImGui::Button("Settings", ImVec2(100, 20)))
    {
        ImGui::OpenPopup("settings_popup");

        //Clears saving and loading info that is next to saving and loading buttons
        LoadingOutput = "";
        SavingOutput = "";
    }


    ImGui::SetNextWindowPos(ImVec2(120, 100));
    ImGui::SetNextWindowSize(ImVec2(370, 260));

    //Settings window
    //-----------------------------------------------------------------------------------------------------------------
    if (ImGui::BeginPopup("settings_popup", ImGuiWindowFlags_MenuBar))
    {

        if (ImGui::BeginTabBar("##tabs"))
        {
            //Begins tab for general settings
            //-------------------------------------------------------------
            if (ImGui::BeginTabItem("General"))
            {
                ImGui::Checkbox("Simulation", &UseSim);
                ImGui::Checkbox("Orthographic 3D view", &Ortho);

                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();

                ImGui::DragFloat("Set simulation speed", &SimSpeed, 0.002f);
                ImGui::Text("Ctrl + click to input precise value");

                ImGui::EndTabItem();
            }

            //Begins tab for saving system
            //-----------------------------------------------------------------------------------------------
            if (ImGui::BeginTabItem("Saving"))
            {
                ImGui::Text("Saving map");

                //InputText to get user input and send it when pressing save button to saveMap function
                ImGui::InputText("Savefile name", fileName, IM_ARRAYSIZE(fileName));
                if (ImGui::Button("Save"))
                {
                    saveMap(fileName);
                } 
                ImGui::SameLine();
                ImGui::Text(SavingOutput);

                ImGui::Spacing();
                ImGui::Spacing();

                ImGui::Text("Loading map");

                //Goes over every file inside resources/saves/ and puts it inside items vector 
                std::vector<std::string> items;
                std::string path = "resources/saves/";
                for (const auto& entry : std::filesystem::directory_iterator(path)) 
                {
                    std::string filename = entry.path().filename().string();

                    //Checks if file is .txt and removes the .txt from string end
                    if (filename.substr(filename.size() - 4, filename.size()) == ".txt")
                        filename.erase(filename.size() - 4, filename.size());
                        items.push_back(filename);
                }

                //Creates combobox to show selected savefile
                if (ImGui::BeginCombo("##combo", current_item.c_str()))
                {
                    for (int n = 0; n < items.size(); n++)
                    {
                        bool is_selected = (current_item == items[n]);
                        if (ImGui::Selectable(items[n].c_str(), is_selected))
                            current_item = items[n].c_str();
                            if (is_selected)
                                ImGui::SetItemDefaultFocus(); 
                    }
                    ImGui::EndCombo();
                }

                //Checks if Default save is selected and if not renders delete button
                if (current_item != "Defaultsave")
                {
                    ImGui::SameLine();
                    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0, 0.6f, 0.6f));

                    //Opens delete verify popup
                    if (ImGui::Button("Delete"))
                        ImGui::OpenPopup("SurePopUp");
                    
                    ImGui::PopStyleColor(1);

                }

                // Popup that verifies that you really want to delete savefile
                if (ImGui::BeginPopup("SurePopUp", ImGuiWindowFlags_MenuBar))
                {
                    ImGui::Text("Are you sure?\nPressing 'Yes' will delete this savefile!!");
                    if (ImGui::Button("No                 "))
                        ImGui::CloseCurrentPopup();

                    ImGui::SameLine();
                    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0, 0.6f, 0.6f));
                    if (ImGui::Button("Yes"))
                    {
                        //Deletes selected file
                        std::string remo = "resources/saves/" + current_item + ".txt";
                        std::remove(remo.c_str());
                        current_item = "Defaultsave";
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::PopStyleColor(1);

                    ImGui::EndPopup();
                }

                //Sends signal to load map
                if (ImGui::Button("Load"))
                {
                    loadMap(current_item.c_str());
                }
                ImGui::SameLine();
                ImGui::Text(LoadingOutput);

                ImGui::Spacing();
                ImGui::Spacing();

                ImGui::Text("You can change the startup save by saving to file\n'Defaultsave' or by going to ''gamefolder/resour-\nces/saves/'' and renaming your save to\n'Defaultsave.txt' (having two same name files\nis probablpy a bad iedea)");

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::EndPopup();
    }
 

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

// Map saving system
void Gui::saveMap(const char* path)
{

    std::ofstream file;
    std::string saveFile = "resources/saves/";
    saveFile = saveFile + path;
    saveFile = saveFile + ".txt";

    // Opens file and clears it
    file.open(saveFile, std::ofstream::out | std::ofstream::trunc);
    file.clear();

    std::vector<std::vector<unsigned int>>& Map = *gameMap;

    // Writes the data to the .txt file
    for (const auto& row : Map) {
        for (const auto& value : row) {
            file << value << ',';
        }
        file << '\n';
    }
    file.close();

    //Sends info next to the save button
    SavingOutput = "Saving ended";
    LoadingOutput = "";

}

void Gui::loadMap(const char* path)
{
    // Clears the map array so .txt file wont begome really big
    gameMap->clear();

    std::string loadFile = "resources/saves/";
    loadFile = loadFile + path;
    loadFile = loadFile + ".txt";

    // Opens savefile
    std::ifstream file(loadFile);
    if (!file.is_open()) 
    {
        // gives error if can't open
        std::string ErrorString = "Could not open file = '' resources/saves/";
        ErrorString = ErrorString + path;
        ErrorString = ErrorString + ".txt ''";

        LoadingOutput = ErrorString.data();
    }

    std::string line;
    // Reads the data from the file
    while (std::getline(file, line)) {
        std::vector<unsigned int> row;
        std::istringstream iss(line);
        std::string cell;
        while (std::getline(iss, cell, ',')) {
            row.push_back(std::stoul(cell));
        }
        gameMap->push_back(row);
    }

    file.close();

    //Sends message 
    LoadingOutput = "Loaded map";
    SavingOutput = "";
}