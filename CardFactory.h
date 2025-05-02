#pragma once

#include <unordered_map>
#include <fstream>
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <nlohmann/json.hpp>

#include "cards.h"
#include "Board.h"

// using json = nlohmann::json;

using CardFactoryFunc = std::function<std::unique_ptr<BaseCard>(const nlohmann::json&)>;

class CardFactory 
{
	public:
	static CardFactory& instance() {
		static CardFactory inst;
		return inst;
	}
	
	void register_type(const std::string& type, CardFactoryFunc func) {
		registry[type] = std::move(func);
	}
	
	std::unique_ptr<BaseCard> create(const nlohmann::json& j) {
		auto it = registry.find(j.value("type", ""));
		if (it != registry.end()) {
			return it->second(j);
		}
		throw std::runtime_error("Unknown card type: " + j.value("type", "unknown"));
	}
	
	private:
	std::unordered_map<std::string, CardFactoryFunc> registry;
};

namespace CardFactoryFunctions
{
	void register_card_types();
	const std::unordered_map<std::string, std::vector<int>> attack_map = {
		{"Attack_None",           {}},
		{"Attack_Forwards",       {1}},
		{"Attack_Left",           {8, 1}},
		{"Attack_Right",          {1, 2}},
		{"Attack_All",            {8, 1, 2}},
		{"Attack_Far_Left",       {10, 11}},
		{"Attack_Far_Right",      {9, 10}},
		{"Attack_Far_All",        {9, 10, 11}},
		{"Attack_Double_Forwards",{1, 10}},
		{"Attack_Double_Left",    {2, 12}}
	};
	std::vector<Player> create_players_from_json(const std::string& filename);

}



// std::unordered_map<std::string, std::unique_ptr<BaseCard>> load_cards_from_json(const std::string& filename) {
// 	std::unordered_map<std::string, std::unique_ptr<BaseCard>> cards;
	
// 	std::ifstream in(filename);
// 	if (!in) throw std::runtime_error("Could not open card JSON file.");
// 	nlohmann::json card_data; in >> card_data;
	
// 	for (const auto& j : card_data) {
// 		auto card = CardFactory::instance().create(j);
// 		cards[card->name] = std::move(card);
// 	}
	
// 	return cards;
// }