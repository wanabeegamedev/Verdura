//
// Created by hous on 12/25/24.
//

#ifndef GAMEUI_H
#define GAMEUI_H



#include <string>
#include <vector>

#include "imgui.h"

class GameUI {
public:
    GameUI() = default;
    std::string text = "Hello UI";
    bool show_welcome = false;
    bool show_help = false;
    bool show_quit = false;
    bool show_about = false;
    bool show_inventory = false;
    void show_welcome_window() ;

    void handleInputs();

    void update();

};
inline void GameUI::show_welcome_window() {
    if (show_welcome)
    {
        ImGui::Begin("Welcome", &show_welcome);
        ImGui::Text("Hello Adventurer, You must Fight !");
        if (ImGui::Button("Close"))
            show_welcome = false;
        ImGui::End();
    }
}

inline void GameUI::handleInputs() {
    if (ImGui::IsKeyPressed(ImGuiKey_W))
        show_welcome = !show_welcome;
    update();
}

inline void GameUI::update() {
    show_welcome_window();
    //show_inventory();
}
#endif //GAMEUI_H
