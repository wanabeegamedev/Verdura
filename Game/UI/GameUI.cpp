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
        ImGui::Text("Level");
        ImGui::SameLine();
        ImGui::Text(std::to_string(stats->currentLvl).c_str());
        //TODO
        ImGui::Text("Unlocked classes:");
        for (auto& _class:warriorClasses) {
            //static_assert()
            ImGui::Text(_class->name.c_str());
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
void GameUI::setHero(Hero* _hero) {
    hero = _hero;
}
void GameUI::show_info_once_window(){
    if (show_info_once)
    {
        ImGui::Begin("Info", &show_info_once);
        ImGui::Text(info.c_str(),0);
        if (ImGui::Button("J'ai compris"))
        {
            show_info_once = false;
            
            if (stateFlag==2)
                over=true;
            else
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
        {
            show_info_log = false;
            
        }
            
        ImGui::End();
    }
}
void GameUI::handleInputs(float deltatime) {
    if (ImGui::IsKeyPressed(ImGuiKey_W))
        show_welcome = !show_welcome;
   /* if (ImGui::IsKeyPressed(ImGuiKey_R))
        show_reward_choice = !show_reward_choice;*/
    update();
}
void GameUI::toggle_info_once_window(const std::string &_info) {
    info = _info;
    show_info_once = true;
    ImGui::SetNextWindowPos(ImVec2(500, 200));
    if (stateFlag != 2) // cas spécial game over
        stateFlag = 1; //playing
}
void GameUI::load_ui_elements() {
    int my_image_width = 0;
    int my_image_height = 0;
    bool ret;
    ret = LoadTextureFromFile("assets/fireball.png",
        &fire_texture_id, &my_image_width, &my_image_height);
    IM_ASSERT(ret);
    LoadTextureFromFile("assets/mana.png",
        &mana_texture_id, &my_image_width, &my_image_height);
    IM_ASSERT(ret);
    LoadTextureFromFile("assets/potion.png",
        &health_texture_id, &my_image_width, &my_image_height);
    IM_ASSERT(ret);
}
void GameUI::add_class_to_track(WarriorClass *_class) {
    warriorClasses.push_back(_class);
}

void GameUI::show_reward_window() {
    if (show_reward_choice)
    {
        std::string last_text;
        ImGui::Begin("Vous Gagnez un niveau pour vos effort, une récompense aussi !");
        if (ImGui::ImageButton("choice2", mana_texture_id, texture_size, uv0, uv1, bg_col, tint_col)) {
            choice = 2;
        text  = "Obtenez un peu de mana ! ";
        }
        ImGui::SameLine();
        if (ImGui::ImageButton("choice3", health_texture_id, texture_size, uv0, uv1, bg_col, tint_col))
        {
            choice = 3;
            text = "Obtenez un peu de santé ! ";
        }
        ImGui::SameLine();
        if (warriorClasses.size() < WARRIOR_CLASSES_COUNT)
        {
            if (ImGui::ImageButton("choice1", fire_texture_id, texture_size, uv0, uv1, bg_col, tint_col)) {
                choice = 1;
                text = "Débloquez la classe Sorcier! ";
            }
        }
        last_text = text;
        ImGui::Text(last_text.c_str());
        if (ImGui::Button("Confirmer"))
        {
            show_reward_choice = false;
            stateFlag = 0 ;
            confirmed_choice = choice;
            if (confirmed_choice == 1 ) {
                hero->HeroClasses.emplace_back(rewardClass);
                add_class_to_track(rewardClass);
            }
            if (confirmed_choice == 2 ) {
                stats->currentMana += 100;
            }
            if (confirmed_choice == 3 ) {
                stats->currentHp += 150;

            }
            stats->currentLvl++;
            confirmed_choice = 0;
            choice = 0;
        }
        ImGui::End();
    }
}

void GameUI::toggle_info_reward_window() {
    show_reward_choice = true;

    stateFlag = 1;
}
inline void GameUI::update() {

    show_welcome_window();
    show_info_once_window();
    show_reward_window();
    show_stats();
}
