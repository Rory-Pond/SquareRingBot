#include <chrono>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "Board.h"
#include "main.h"
#include "CardFactory.h"
#include "CardLibrary.h"
#include "libary.h"
#include "MCTSBot.h"
#include "Monte.h"
#include "Random.h"


void CardFactoryFunctions::register_card_types() 
{
	CardFactory::instance().register_type("BaseCard", [](const nlohmann::json& j) {
		std::string name = j.value("name", "Unnamed_Card");
		return std::make_unique<BaseCard>(name);
	});
	
	CardFactory::instance().register_type("MoveCard", [](const nlohmann::json& j) {
		std::string name = j.value("name", "Unnamed_Card");
		int rotate = j.value("rotate", 0);
		int move = j.value("move", 0);
		return std::make_unique<MoveCard>(name, rotate, move);
	});
	
	CardFactory::instance().register_type("TokenCard", [](const nlohmann::json& j) {
		std::string name = j.value("name", "Unnamed_Card");
		std::string token_string = j.value("token", "Token_None");
		TokenType token;
		if (token_string == "None") {
			token = TokenType::None;
		} else if (token_string == "Burst") {
			token = TokenType::Burst;
		} else if (token_string == "React") {
			token = TokenType::React;
		} else {
			throw std::runtime_error("Unknown token type: " + token_string);
		}
		return std::make_unique<TokenCard>(name, token);
	});
	
	CardFactory::instance().register_type("AttackCard", [](const nlohmann::json& j) {
		std::string name = j.value("name", "Unnamed_Attack");
		std::string attack_target_name = j.value("attack_target", "Attack_None");
		std::vector<int> attack_target = CardFactoryFunctions::attack_map.at(attack_target_name);
		int damage = j.value("damage", 0);
		std::vector<std::string> attack_move = j.value("attack_move", std::vector<std::string>{});
		bool cooldown = j.value("cooldown", false);
		bool daze = j.value("daze", false);
		
		return std::make_unique<AttackCard>(name, attack_target, damage, attack_move, cooldown, daze);
	});
	
	CardFactory::instance().register_type("AttackCardWithEffect", [](const nlohmann::json& j) {
		std::string name = j.value("name", "Unnamed_AttackEffect");
		std::vector<int> attack_target = j.value("attack_target", std::vector<int>{});
		int damage = j.value("damage", 0);
		std::vector<std::string> attack_move = j.value("attack_move", std::vector<std::string>{});
		bool cooldown = j.value("cooldown", false);
		bool daze = j.value("daze", false);
		
		return std::make_unique<AttackCardWithEffect>(name, attack_target, damage, attack_move, cooldown, daze);
	});
	
	CardFactory::instance().register_type("AttackCardWithPreviousCardEffect", [](const nlohmann::json& j) {
		std::string name = j.value("name", "Unnamed_PreviousEffect");
		std::vector<int> attack_target = j.value("attack_target", std::vector<int>{});
		int damage = j.value("damage", 0);
		std::vector<std::string> attack_move = j.value("attack_move", std::vector<std::string>{});
		bool cooldown = j.value("cooldown", false);
		bool daze = j.value("daze", false);
		std::string previous_card_name = j.value("previous_card_name", "");
		int bonus_damage = j.value("bonus_damage", 0);
		bool bonus_daze = j.value("bonus_daze", false);
		
		return std::make_unique<AttackCardWithPreviousCardEffect>(
			name, attack_target, damage, attack_move, cooldown, daze,
			previous_card_name, bonus_damage, bonus_daze
		);
	});
	
	CardFactory::instance().register_type("AttackCardWithRoundEffect", [](const nlohmann::json& j) {
		std::string name = j.value("name", "Unnamed_RoundEffect");
		std::vector<int> attack_target = j.value("attack_target", std::vector<int>{});
		int damage = j.value("damage", 0);
		std::vector<std::string> attack_move = j.value("attack_move", std::vector<std::string>{});
		bool cooldown = j.value("cooldown", false);
		bool daze = j.value("daze", false);
		std::array<int, 3> round_damage = {0, 0, 0};
		
		if (j.contains("round_damage")) {
			const auto& rd = j["round_damage"];
			for (size_t i = 0; i < std::min(rd.size(), round_damage.size()); ++i) {
				round_damage[i] = rd[i].get<int>();
			}
		}
		
		return std::make_unique<AttackCardWithRoundEffect>(
			name, attack_target, damage, attack_move, cooldown, daze, round_damage
		);
	});

	CardFactory::instance().register_type("DamageModificationCard", [](const nlohmann::json& j) {
		std::string name = j.value("name", "Unnamed_DamageModifier");
		int damageModifier = j.value("damageModifier", 0);
		bool cannotBeDazed = j.value("cannotBeDazed", false);
		bool increaseBurst = j.value("increaseBurst", false);
	
		return std::make_unique<DamageModificationCard>(name, damageModifier, cannotBeDazed, increaseBurst);
	});
}

std::vector<Player> CardFactoryFunctions::create_players_from_json(const std::string& filename) 
{
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open player JSON file: " + filename);
    }

    nlohmann::json json_data;
    try {
        file >> json_data;
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to parse JSON: " + std::string(e.what()));
    }

    if (!json_data.is_array()) {
        throw std::runtime_error("Player JSON must be an array.");
    }

    std::vector<Player> players;

    for (const auto& player_json : json_data) {
        try {
            // Read player attributes from JSON
            std::string name = player_json.value("name", "Unnamed_Player");
            int starting_health = player_json.value("starting_health", 15);
            int starting_burst = player_json.value("starting_burst", 2);
            int starting_react = player_json.value("starting_react", 2);
            int health_regen = player_json.value("health_regen", 2);

            // Read card names and convert them to BaseCard* using CardLibrary
            std::vector<BaseCard*> deck;
            for (const auto& card_name : player_json.at("cards")) {
                BaseCard* card = CardLibrary::instance().get_card(card_name);
                if (!card) {
                    throw std::runtime_error("Card not found: " + card_name);
                }
                deck.push_back(card);
            }

            // Create a Player object and add it to the vector
            players.emplace_back(name, starting_health, starting_burst, starting_react, health_regen, deck);
        } catch (const std::exception& e) {
            std::cerr << "Error creating player: " << e.what() << std::endl;
        }
    }

    return players;
}