#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "CardLibrary.h"
#include "CardFactory.h"

void CardLibrary::load_from_json(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open card library JSON: " << filename << std::endl;
        return;
    }

    nlohmann::json data;
    try {
        file >> data;
    } catch (const std::exception& e) {
        std::cerr << "Failed to parse JSON: " << e.what() << std::endl;
        return;
    }

    if (!data.is_array()) {
        std::cerr << "Card library JSON must be an array.\n";
        return;
    }

    for (const auto& card_json : data) {
        try {
            std::string name = card_json.value("name", "");
            std::string type = card_json.value("type", "");

            if (name.empty() || type.empty()) {
                std::cerr << "Card missing name or type, skipping...\n";
                continue;
            }

            auto card = CardFactory::instance().create(card_json);
            if (!card) {
                std::cerr << "Factory failed to create card of type " << type << " for " << name << "\n";
                continue;
            }

            cards[name] = std::move(card);
        } catch (const std::exception& e) {
            std::cerr << "Error processing card: " << e.what() << "\n";
        }
    }

    std::cout << "Loaded " << cards.size() << " cards from " << filename << "\n";
}
