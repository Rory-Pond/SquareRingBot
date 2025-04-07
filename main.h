#pragma once
#include <iostream>
#include <vector>
#include <string>

#include "constants.h"

class Card
{
	private:
	public:
		std::string id;
		std::string name;
		int rotate = 0;
		int move = 0;
		int attack_move = 0;
		int attack_rotate = 0;
		std::vector<int> attack;
		int damage = 0;
		Card(std::string card_id = "AA", std::string card_name = "null", int r = 0, int m = 0, const std::vector<int>& a = {}, int d = 0)
			: id(card_id), name(card_name), rotate(r), move(m), attack(a), damage(d) {}
};

class Action
{
	private:
	public:
		std::string name;
		std::string id;
		TokenType token = TokenType::Not_Submitted;
		int rotate = 0;
		int move = 0;
		int attack_move = 0;
		int attack_rotate = 0;
		int damage = 0;
		std::vector<int> attack;
		Action(std::string card_id = "AA", std::string card_name = "null", int r = 0, int m = 0, const std::vector<int>& a = {}, int d = 0, TokenType tok = TokenType::Not_Submitted)
			
		: id(card_id), name(card_name), rotate(r), move(m), attack(a), damage(d), token(tok) {}
};

inline bool operator==(const Action& lhs, const Action& rhs) {
	return lhs.id == rhs.id;
};
	
class Player 
{
	public:
		int health = 15;
		int round_wins = 0;
		vec2 position;
		int direction = 0;
		bool is_dazed;
		std::vector<Action> deck;
		std::vector<int> not_usable_card;
		int statusEffects = 0;
		int regen = 2;
		int burst_count  = 2;
		int react_count = 2;
		
		void win_round();
		void reset_round(bool isPlayerRed);
		std::vector<std::vector<Action>> get_all_moves() const;
		std::vector<std::vector<Action>> get_all_actions_for_cards(int card_count) const;
		std::vector<std::vector<Action>> get_all_actions_for_cards(int card_count, std::vector<std::vector<Action>> not_allowed) const;
		std::vector<std::vector<Action>> get_all_token_moves() const;

		Player(int h, vec2 pos, int dir, const std::vector<Action>& crds)
		: health(h), position(pos), direction(dir), deck(crds) {}

		std::string character_sprite;
};



class Board
{
	private:
	public:
		std::vector<std::vector<Action>> get_all_moves(bool isPlayerRed) const;
		int turn_count = 0;
		Player player_red, player_blue;
		Board(Player p_1, Player p_2)
		:player_red(p_1), player_blue(p_2){}

		TurnOrder turn_order = Token;
		void printGrid();
		void resolveTokens();
		void ResolveTurn();
		bool red_player_revealed = false;
		bool blue_player_revealed = false;

		TokenType red_player_token = TokenType::Not_Submitted;
		TokenType blue_player_token = TokenType::Not_Submitted;
		
		std::vector<Action> player_red_current_action, player_blue_current_action;
		
		void submit_turn_action(const std::vector<Action>& action, bool isPlayerRed);
		void playout_action(const Action& pRedCard, const Action& pBlueCard);
		void play_action(std::vector<Action> pRed_cards, std::vector<Action> pBlue_cards);
		float eval(bool isPlayerRed = true);
		int game_winner(bool isPlayerRed = true);
		bool game_over;
		bool is_game_over(){return game_over;};
		bool is_round_over(int round){return round * 6 > turn_count;};
		int current_round(){return 1 + turn_count/6;};
		int whos_turn(bool isPlayerRed = true);



};

class Bot 
{
	public:
		virtual std::vector<Action> find_best_move(const Board& board, bool isPlayerRed) = 0;
		// virtual std::vector<Action> find_best_move(const Board& board, bool isPlayerRed)
		// {
		// 	return {Action()};
		// }
		std::string name;
};

class BestCurrent: public Bot
{
	public:
	std::vector<Action> find_best_move(const Board& board, bool isPlayerRed);

};


class MinMax: public Bot
{
	public:
	std::vector<Action> find_best_move(const Board& board, bool isPlayerRed);
	static size_t randomIndex(size_t size);
	static int minimax(
		Board& board, 
		const std::vector<Action>& move1, 
		const std::vector<Action>& move2, 
		int depth, bool isMaximizing, 
		int alpha, int beta);

};