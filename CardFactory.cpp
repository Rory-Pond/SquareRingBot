#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <chrono>

#include "main.h"
#include "libary.h"
#include "MCTSBot.h"
#include "Random.h"
#include "Monte.h"

#include "CardLibrary.h"
#include "CardFactory.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

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