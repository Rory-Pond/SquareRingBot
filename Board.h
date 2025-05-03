#pragma once
#include <vector>
#include <string>
#include <optional>

#include "Player.h"

class BaseCard;

class Board
{
	private:
	public:
		std::vector<std::vector<BaseCard*>> get_all_moves(bool isPlayerRed) const;
		int turn_count = 0;
		Player player_red, player_blue;
		Board(Player p_1, Player p_2)
		: player_red(p_1), player_blue(p_2)
		{
			player_red.deck = p_1.deck;
			player_blue.deck = p_2.deck;
			player_red.position = vec2(1, 1);
			player_blue.position = vec2(3, 3);
			player_red.direction = 1;
			player_blue.direction = 5;
		}

		TurnOrder turn_order = Token;
		void printGrid();
		void resolveTokens();
		void ResolveTurn();
		void FinishTurn();
		void SetCards();
		bool red_player_revealed = false;
		bool blue_player_revealed = false;
		int card_index = 0;

		TokenType red_player_token = TokenType::Not_Submitted;
		TokenType blue_player_token = TokenType::Not_Submitted;
		
		// std::array<BaseCard*, 3> red_player_cards;
		// std::array<BaseCard*, 3> blue_player_cards;
		
		std::vector<BaseCard*> red_player_cards, blue_player_cards;
		std::optional<BaseCard*> red_attack_move, blue_attack_move;

		std::vector<std::vector<BaseCard*>> red_attack_move_options, blue_attack_move_options;
		std::vector<std::vector<BaseCard*>> GenerateAttackMoveOptions(std::vector<std::string> card_names);
		std::vector<std::vector<BaseCard*>> LookUpBasicCards(std::vector<std::string> card_names);

		void makeValidMove(int red_move, int blue_move);
		void submit_turn_action(const std::vector<BaseCard*> action, bool isPlayerRed);
		void playout_action(const BaseCard* red_card, const BaseCard* blue_card);

		float eval(bool isPlayerRed = true);
		int game_winner(bool isPlayerRed = true);
		bool game_over = false;
		bool is_game_over() const {return game_over;};
		bool is_round_over(int round){return round * 6 > turn_count;};
		int current_round(){return 1 + turn_count/6;};
		int whos_turn(bool isPlayerRed = true);
};