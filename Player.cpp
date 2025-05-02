#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <map>
#include <functional>
#include <algorithm>

#include "main.h"
#include "Board.h"


std::vector<std::vector<BaseCard*>> get_all_moves_recursive(std::vector<BaseCard*> deck, int depth) {
	std::vector<std::vector<BaseCard*>> moves;
	std::vector<BaseCard*> current_combination;

	// Helper function for recursion
	std::function<void(int)> generate_combinations = [&](int start) {
		if (current_combination.size() == depth) {
			moves.push_back(current_combination);
			return;
		}

		for (int i = 0; i < deck.size(); ++i) {
			// Ensure no duplicate indices in the combination
			if (std::find(current_combination.begin(), current_combination.end(), deck[i]) == current_combination.end()) 
			{
				current_combination.push_back(deck[i]);
				generate_combinations(i + 1);
				current_combination.pop_back();
			}
		}
	};

	generate_combinations(0);
	return moves;
}


std::vector<std::vector<BaseCard*>> Player::get_all_actions_for_cards(int card_count) const
{
	return get_all_moves_recursive(deck, card_count);
}

std::vector<std::vector<BaseCard*>> Player::get_all_actions_for_cards(int card_count, std::vector<std::vector<BaseCard*>> not_allowed) const
{
	std::vector<BaseCard*> deck_copy = deck;
	for(auto action : not_allowed)
	{
		for(auto card : action)
		{
			auto it = std::find(deck_copy.begin(), deck_copy.end(), card);
			if(it != deck_copy.end())
			{
				deck_copy.erase(it);
			}
		}
	}

	return get_all_moves_recursive(deck_copy, card_count);
}

std::vector<std::vector<BaseCard*>> Player::get_all_moves() const
{
	return get_all_moves_recursive(deck, 2);
}

void Player::win_round()
{
	round_wins++;
}
void Player::reset_round(bool isPlayerRed)
{
	position = isPlayerRed ? vec2(1,1) : vec2(3,3);
	direction = isPlayerRed ? 1 : 5;
	health = health + regen;
}


/*


std::vector<std::vector<BaseCard*>> get_all_moves_loop_1(std::vector<BaseCard*> deck)
{
	std::vector<std::vector<BaseCard*>> moves;
	for (int i = 0; i < deck.size(); ++i)
	{
		moves.push_back({deck[i]});
	}
	return moves;
}
std::vector<std::vector<BaseCard*>> get_all_moves_loop_2(std::vector<BaseCard*> deck)
{
	std::vector<std::vector<BaseCard*>> moves;
	for (int i = 0; i < deck.size(); ++i)
	{
		for (int j = 0; j < deck.size(); ++j)
		{
			if(i!=j)
			{
				moves.push_back({deck[i], deck[j], });
			}
		}
	}
	return moves;
}
std::vector<std::vector<BaseCard*>> get_all_moves_loop_3(std::vector<BaseCard*> deck)
{
	std::vector<std::vector<BaseCard*>> moves;
	for (int i = 0; i < deck.size(); ++i)
	{
		for (int j = 0; j < deck.size(); ++j)
		{
			if(i!=j)
			{
				for (int k = 0; k < deck.size(); ++k)
				{
					if(k!=i && k!=j)
					{
						moves.push_back({deck[i], deck[j], deck[k]});
					}
				}
			}
		}
	}
	return moves;
}

std::string deck_hash(std::vector<BaseCard*> deck)
{
	std::string hash;
	for(auto card : deck)
	{
		hash += card.id;
	}
	return hash;
}

std::vector<std::vector<BaseCard*>> get_all_moves_look_up(std::vector<BaseCard*> deck)
{
	static std::map<std::string, std::vector<std::vector<BaseCard*>>> move_map;
	auto key = deck_hash(deck);
	if(!move_map.contains(key))
	{
		std::cout << "new deck: " << key << std::endl;
		move_map[key] = get_all_moves_loop(deck);
	}

	return move_map[key];
}

*/