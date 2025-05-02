#pragma once

#include <nlohmann/json.hpp>

#include "Player.h"

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