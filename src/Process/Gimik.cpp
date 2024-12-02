#include "../core/Gimik.h"
#include <string>
#include <sstream>

Gimik::Gimik(RBTree& tree) : rbtree(tree), rng(std::random_device{}()) {}

float Gimik::calculate() {
    switch(operation) {
        case '+': return num1 + num2;
        case '-': return num1 - num2;
        case '*': return num1 * num2;
        case '/': return num2 != 0 ? num1 / num2 : 0;
        default: return 0;
    }
}

int Gimik::rollDice() {
    std::uniform_int_distribution<> dist(1, 6);
    return dist(rng);
}

void Gimik::renderCalculator() {
    ImGui::Text("Calculator");
    ImGui::Separator();
    
    ImGui::InputFloat("Number 1", &num1);
    ImGui::InputFloat("Number 2", &num2);
    
    ImGui::Text("Operation:");
    ImGui::SameLine();

    if (ImGui::Button("+")) operation = '+';
    ImGui::SameLine();
    if (ImGui::Button("-")) operation = '-';
    ImGui::SameLine();
    if (ImGui::Button("*")) operation = '*';
    ImGui::SameLine();
    if (ImGui::Button("/")) operation = '/';
    
    if (ImGui::Button("Calculate")) {
        // Format input for searchValue
        std::string calcStr = "calc:" + 
                             std::to_string(num1) + ":" + 
                             operation + ":" + 
                             std::to_string(num2);
        
        // Use searchValue
        std::vector<Node*> results = rbtree.searchValue(calcStr);
        
        // Display result if available
        if (!results.empty()) {
            result = std::stof(results[0]->data);
        }
    }
    
    ImGui::Text("Result: %.2f", result);
}

void Gimik::renderDiceRoller() {
    ImGui::Text("Dice Roller");
    ImGui::Separator();
    
    if (ImGui::Button("Roll Dice")) {
        isRolling = true;
        rollDuration = 1.0f;
    }
    
    if (isRolling) {
        rollDuration -= ImGui::GetIO().DeltaTime;
        diceResult = rollDice();
        
        if (rollDuration <= 0) {
            isRolling = false;
        }
    }
    
    ImGui::Text("Result: %d", diceResult);
    
    // Visual dice representation
    ImGui::Text(isRolling ? "Rolling..." : "[ %d ]", diceResult);
}

void Gimik::renderRandomBook() {
    ImGui::Text("Random Book Picker");
    ImGui::Separator();
    
    if (ImGui::Button("Get Random Book")) {
        std::string random = "random";
        randomBookResult = rbtree.searchValue(random);
    }
    
    if (!randomBookResult.empty()) {
        std::string data = randomBookResult[0]->data;
        std::stringstream ss(data);
        std::vector<std::string> fields;
        std::string field;
        
        while (std::getline(ss, field, '|')) {
            fields.push_back(field);
        }
        
        if (fields.size() >= 3) {
            ImGui::Text("ISBN: %s", fields[0].c_str());
            ImGui::Text("Title: %s", fields[1].c_str());
            ImGui::Text("Author: %s", fields[2].c_str());
        }
    }
}

void Gimik::render() {
    if (ImGui::Button("Fun Features")) {
        ImGui::OpenPopup("Gimik Features");
    }
    
    if (ImGui::BeginPopupModal("Gimik Features", nullptr, 
        ImGuiWindowFlags_AlwaysAutoResize)) {
        
        if (ImGui::BeginTabBar("GimikTabs")) {
            if (ImGui::BeginTabItem("Calculator")) {
                renderCalculator();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Dice Roller")) {
                renderDiceRoller();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Random Book")) {
                renderRandomBook();
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        
        if (ImGui::Button("Close")) {
            ImGui::CloseCurrentPopup();
        }
        
        ImGui::EndPopup();
    }
}