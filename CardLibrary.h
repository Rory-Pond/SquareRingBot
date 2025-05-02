#pragma once

#include "Cards.h"

class CardLibrary {
public:
    static CardLibrary& instance() {
        static CardLibrary lib;
        return lib;
    }

    BaseCard* get_card(const std::string& name) const {
        auto it = cards.find(name);
        if (it != cards.end()) {
            return it->second.get();
        }
        throw std::runtime_error("Card not found: " + name);
    }

    void load_from_json(const std::string& filename); // assumes you implement this

private:
    std::unordered_map<std::string, std::unique_ptr<BaseCard>> cards;

    CardLibrary() = default;
    CardLibrary(const CardLibrary&) = delete;
    CardLibrary& operator=(const CardLibrary&) = delete;
    friend const BaseCard* library(const std::string&); // allow access to private
};

inline BaseCard* find_card(const std::string& name) {
    return CardLibrary::instance().get_card(name);
}
