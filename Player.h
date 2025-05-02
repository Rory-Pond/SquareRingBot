#pragma once
#include <vector>
#include <string>

#include "Constants.h"

class BaseCard;

class Player 
{
	public:
		std::string name;	
		int health = 15;
		int round_wins = 0;
		vec2 position;
		int direction = 0;
		bool is_dazed = false;
		std::vector<BaseCard*> deck;
		std::vector<std::string> not_usable_card;
		int statusEffects = 0;
		int regen = 2;
		int burst_count = 2;
		int react_count = 2;

		void win_round();
		void reset_round(bool isPlayerRed);
		std::vector<std::vector<BaseCard*>> get_all_moves() const;
		std::vector<std::vector<BaseCard*>> get_all_actions_for_cards(int card_count) const;
		std::vector<std::vector<BaseCard*>> get_all_actions_for_cards(int card_count, std::vector<std::vector<BaseCard*>> not_allowed) const;
		std::vector<std::vector<BaseCard*>> get_all_token_moves() const;

		Player(int h, vec2 pos, int dir, const std::vector<BaseCard*>& crds)
		: health(h), position(pos), direction(dir), deck(crds) {}
		Player(std::string name, int health = 15, int burst_count = 2, int react_count = 2, int regen = 2, std::vector<BaseCard*> deck = {})
		:name(name), health(health), burst_count(burst_count), react_count(react_count), regen(regen), deck(deck) {}


};