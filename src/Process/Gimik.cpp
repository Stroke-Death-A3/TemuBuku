#include "../core/Gimik.h"
#include <string>
#include <sstream>

Gimik::Gimik(RBTree& tree) : rbtree(tree), rng(std::random_device{}()) {}



int Gimik::rollDice() {
    std::uniform_int_distribution<> dist(1, 6);
    return dist(rng);
}

void Gimik::renderCalculator() {
    ImGui::Text("Calculator");
    ImGui::Separator();
    
    // Dynamic number inputs
    static char expr[256] = "";
    ImGui::InputText("Expression", expr, sizeof(expr));
    
    if (ImGui::Button("Calculate")) {
        expression = expr;
        std::stringstream ss(expression);
        float num;
        char op;
        
        // Parse first number
        ss >> num;
        result = num;
        
        // Parse operations and numbers
        while (ss >> op >> num) {
            switch(op) {
                case '+': result += num; break;
                case '-': result -= num; break;
                case '*': result *= num; break;
                case '/': result = (num != 0) ? result / num : 0; break;
            }
        }
    }
    
    ImGui::Text("Result: %.2f", result);
    
    // Quick operation buttons
    ImGui::Text("Quick Operations:");
    if (ImGui::Button("+")) strcat(expr, " + ");
    ImGui::SameLine();
    if (ImGui::Button("-")) strcat(expr, " - ");
    ImGui::SameLine();
    if (ImGui::Button("*")) strcat(expr, " * ");
    ImGui::SameLine();
    if (ImGui::Button("/")) strcat(expr, " / ");
    ImGui::SameLine();
    if (ImGui::Button("C")) {
        expr[0] = '\0';
        result = 0;
    }
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