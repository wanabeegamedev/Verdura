//
// Created by hous on 12/25/24.
//

#include "GameUI.h"
#include <cstdint>

#include "imgui_image_loader_opengl.h"


GameUI::GameUI() {
    texture_size = ImVec2(48.0f, 48.0f);
    texture_width = UI_ELEMENT_IMG_WIDTH;
    uv0 = ImVec2(0.0f, 0.0f);
    uv1 = ImVec2(48.0f / texture_width, 48.0f / texture_width);
    bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    load_ui_elements();
}
void GameUI::show_stats() {
    if (stats!=nullptr) {
        ImGui::Begin("Etat de votre Héro : ");
        ImGui::Text("HP");
        ImGui::SameLine();
        ImGui::Text(std::to_string(stats->currentHp).c_str());
        ImGui::Text("MP");
        ImGui::SameLine();
        ImGui::Text(std::to_string(stats->currentMana).c_str());
        ImGui::Text("Unlocked classes:");
        for (auto& _class:warriorClasses) {
            //static_assert()
            ImGui::Text(_class->name);
        }
        ImGui::End();

    }
}
void GameUI::show_welcome_window() {
    if (show_welcome)
    {
        ImGui::Begin("Welcome", &show_welcome);
        ImGui::Text("Hello Adventurer, You must Fight !");
        if (ImGui::Button("Close"))
            show_welcome = false;
        ImGui::End();
    }
}
void GameUI::setStats(Stats* _stats) {
    stats = _stats;
}
void GameUI::show_info_once_window(){
    if (show_info_once)
    {
        ImGui::Begin("Info", &show_info_once);
        ImGui::Text(info.c_str(),0);
        if (ImGui::Button("J'ai compris"))
        {
            show_info_once = false;
            stateFlag = 0;
        }
        ImGui::End();
    }
}
void GameUI::show_info_log_window()
{
    if (show_info_log) {
        ImGui::Begin("Welcome", &show_info_log);
        ImGui::Text("Hello Adventurer, You must Fight !");
        if (ImGui::Button("Close"))
            show_info_log = false;
        ImGui::End();
    }
}
void GameUI::handleInputs(float deltatime) {
    if (ImGui::IsKeyPressed(ImGuiKey_W))
        show_welcome = !show_welcome;
    if (ImGui::IsKeyPressed(ImGuiKey_R))
        show_reward_choice = !show_reward_choice;
    update(deltatime);
}
void GameUI::toggle_info_once_window(const std::string &_info) {
    info = _info;
    show_info_once = true;
    ImGui::SetNextWindowPos(ImVec2(500, 200));
    stateFlag = 1;
}
void GameUI::load_ui_elements() {
    int my_image_width = 0;
    int my_image_height = 0;
    bool ret = LoadTextureFromFile("/home/hous/CLionProjects/Verdura/Game/UI/sword-10-48.png",
        &sword_texture_id, &my_image_width, &my_image_height);
    IM_ASSERT(ret);
}
void GameUI::add_class_to_track(WarriorClass *_class) {
    warriorClasses.push_back(_class);
}

void GameUI::show_reward_window() {
    if (show_reward_choice)
    {
        ImGui::Begin("Choose a reward !");
        if (ImGui::ImageButton("choice1", sword_texture_id, texture_size, uv0, uv1, bg_col, tint_col))
            choice = 1;
        ImGui::SameLine();
        if (ImGui::ImageButton("choice2", sword_texture_id, texture_size, uv0, uv1, bg_col, tint_col))
            choice = 2;
        ImGui::Text("choice = %d", choice);
        if (ImGui::Button("Confirmer"))
        {
            show_reward_choice = false;
            stateFlag = 0 ;
        }
        ImGui::End();
    }
}

void GameUI::toggle_info_reward_window() {
    show_reward_choice = true;
    stateFlag = 1;
}
inline void GameUI::update(float deltatime) {

    show_welcome_window();
    show_info_once_window();
    show_reward_window();
    show_stats();
}
