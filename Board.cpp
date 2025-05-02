#include <unordered_map>
#include <random>
#include <iostream>

#include "Board.h"
#include "Player.h"
#include "CardLibrary.h"

int chebyshev_distance(const vec2& a, const vec2& b) {
	return std::max(std::abs(b.x - a.x), std::abs(b.y - a.y));
}

float Board::eval(bool isPlayerRed)
{
	float distance = chebyshev_distance(player_red.position, player_blue.position)*0.1;
	float healthChange = isPlayerRed ? (player_red.health - player_blue.health ): (player_blue.health - player_red.health);
	return healthChange - distance;
}

void Board::SetCards()
{
	std::random_device rd;
	std::mt19937 g(rd());
	if(player_red.is_dazed)
	{
		//shuffle cards
		std::shuffle(red_player_cards.begin(), red_player_cards.end(), g);
		player_red.is_dazed = false;
	}
	if(player_blue.is_dazed)
	{
		//shuffle cards
		std::shuffle(blue_player_cards.begin(), blue_player_cards.end(), g);
		player_blue.is_dazed = false;
	}
	if(red_player_cards.size() == 2)
	{
		red_player_cards.push_back(find_card("Null"));
	}
	if(blue_player_cards.size() == 2)
	{
		blue_player_cards.push_back(find_card("Null"));
	}
}

void Board::submit_turn_action(const std::vector<BaseCard*> action, bool isPlayerRed)
{
	const BaseCard* first_action = action[0];

	if(turn_order == TurnOrder::Token)
	{
		auto& player_token = isPlayerRed ? red_player_token : blue_player_token;
		player_token = first_action->getToken();

		if (red_player_token != TokenType::Not_Submitted && blue_player_token != TokenType::Not_Submitted) {
			turn_order = TurnOrder::Reveal;
			resolveTokens();
			if (red_player_token != TokenType::React && blue_player_token != TokenType::React) {
				turn_order = TurnOrder::Card;
			}
		}
		return;
	}

	if(turn_order == TurnOrder::Reveal)
	{
		if(isPlayerRed)
		{
			if(blue_player_token == TokenType::React)
			{
				red_player_cards.push_back(action[0]);
			}
			red_player_revealed = true;
		}
		else
		{
			if(red_player_token == TokenType::React)
			{
				blue_player_cards.push_back(action[0]);
			}
			blue_player_revealed = true;
		}
		if(red_player_revealed && blue_player_revealed)
		{
			turn_order = TurnOrder::Card;
			red_player_revealed = false;
			blue_player_revealed = false;
		}
		return;
	}

	if(turn_order == TurnOrder::Card)
	{
		auto& player_cards = isPlayerRed ? red_player_cards : blue_player_cards;
		player_cards.insert(player_cards.end(), action.begin(), action.end());

		if (red_player_cards.size() >= 2 && blue_player_cards.size() >= 2) {
			SetCards();
			ResolveTurn();
		}
		return;
	}

	if(turn_order == TurnOrder::Attack_Move)
	{
		auto& player_attack_move = isPlayerRed ? red_attack_move : blue_attack_move;
		player_attack_move = action[0];

		if (red_attack_move && blue_attack_move) 
		{
			playout_action(*red_attack_move, *blue_attack_move);
			red_attack_move.reset();
			blue_attack_move.reset();
			red_attack_move_options.clear();
			blue_attack_move_options.clear();
			ResolveTurn();
		}
		return;
	}
}

bool randomBool() {
	static std::random_device rd;  // Seed for random number generator
	static std::mt19937 gen(rd()); // Mersenne Twister engine
	static std::uniform_int_distribution<> dis(0, 1); // Uniform distribution between 0 and 1
	return dis(gen) == 1;
}

void Board::ResolveTurn()
{
	while(card_index < 3)
	{
		playout_action(red_player_cards[card_index], blue_player_cards[card_index]);
		if(player_red.health <= 0 || player_blue.health <= 0)
		{
			game_over = true;
		}
		card_index++;
		if(red_attack_move_options.size() == 1 && blue_attack_move_options.size() == 1)
		{
			red_attack_move = red_attack_move_options[0][0];
			blue_attack_move = blue_attack_move_options[0][0];
			playout_action(red_attack_move.value(), blue_attack_move.value());
			red_attack_move.reset();
			blue_attack_move.reset();
			red_attack_move_options.clear();
			blue_attack_move_options.clear();
		}
		if(red_attack_move_options.size() > 1 || blue_attack_move_options.size() > 1)
		{
			turn_order = TurnOrder::Attack_Move;
			return;
		}
	}
	
	// FinishTurn
	turn_count++;
	if(turn_count == 6 || turn_count == 12 || turn_count == 18)
	{
		if(player_red.health>player_blue.health)
		{
			player_red.win_round();
		}
		else if(player_red.health<player_blue.health)
		{
			player_blue.win_round();
		}
		
		if(turn_count == 6)
		{
			game_over = true;
			return;
		}
		player_red.reset_round(true);
		player_blue.reset_round(false);
	}
	red_player_token = TokenType::Not_Submitted;
	blue_player_token = TokenType::Not_Submitted;
	red_player_cards.clear();
	blue_player_cards.clear();
	red_attack_move.reset();
	blue_attack_move.reset();
	red_attack_move_options.clear();
	blue_attack_move_options.clear();
	red_player_revealed = false;
	blue_player_revealed = false;
	turn_order = TurnOrder::Token;
}

void Board::resolveTokens()
{
	if(red_player_token == TokenType::React && blue_player_token == TokenType::React)
	{
		if(randomBool())
		{
			red_player_token = TokenType::None;
		}
		else
		{
			blue_player_token = TokenType::None;
		}
	}
	
	if(red_player_token == TokenType::React)
	{
		player_red.react_count--;
	}
	if(blue_player_token == TokenType::React)
	{
		player_blue.react_count--;
	}
	if(red_player_token == TokenType::Burst)
	{
		player_red.burst_count--;
	}
	if(blue_player_token == TokenType::Burst)
	{
		player_blue.burst_count--;
	}
}

std::vector<std::vector<BaseCard*>> Board::get_all_moves(bool isPlayerRed) const
{
	const TokenType self_token  = isPlayerRed ? red_player_token : blue_player_token;
	const TokenType other_token = isPlayerRed ? blue_player_token : red_player_token;
	
	int card_count = 2;
	if (other_token == TokenType::React) --card_count;
	if (self_token == TokenType::Burst)  ++card_count;

	switch (turn_order) {
		case Token:
		{
			std::vector<std::vector<BaseCard*>> token_options;
			token_options.push_back({find_card("Token_None")});
			
			const Player& current_player = isPlayerRed ? player_red : player_blue;
			if (current_player.react_count > 0) {
				token_options.push_back({find_card("Token_React")});
			}
			if (current_player.burst_count > 0) {
				token_options.push_back({find_card("Token_Burst")});
			}
			return token_options;
		}
		
		case Reveal:
		{
			if(isPlayerRed)
			{
				return (blue_player_token == TokenType::React)
				? player_red.get_all_actions_for_cards(1)
				: std::vector<std::vector<BaseCard*>>{{find_card("Null")}};
			}
			else
			{
				return (red_player_token == TokenType::React)
				? player_blue.get_all_actions_for_cards(1)
				: std::vector<std::vector<BaseCard*>>{{find_card("Null")}};
			}
		}
		
		case Card:
		{
			if(isPlayerRed)
			{
				return player_red.get_all_actions_for_cards(card_count);
			}
			else
			{
				return player_blue.get_all_actions_for_cards(card_count);
			}
		}
		case Attack_Move:
		{
			if(isPlayerRed)
			{
				return red_attack_move_options;
			}
			else
			{
				return blue_attack_move_options;
			}
		}
	}
	return {};
}

bool isOnBoard(vec2 pos)
{
	return (pos.x >= 0 && pos.x <= 4) && (pos.y >= 0 && pos.y <= 4);
}

vec2 positionFromIndex(int n, int rot)
{
	if (n == 0 )
	return {0,0};
	if (n < 9)
	return ONE_SPACE_MOVE[(n-1+rot) % 8];
	
	return TWO_SPACE_MOVE[(n-10+rot*2) % 16];
}

vec2 combinePDM(vec2 pos, int dir, int move)
{
	auto value2 = positionFromIndex(move, dir);
	auto value = pos + positionFromIndex(move, dir);
	return value;
}

void Board::makeValidMove(int red_move, int blue_move)
{
	if(red_move == 0 && blue_move == 0)
	{
		return;
	}
	
	vec2 red_new_pos = combinePDM(player_red.position, player_red.direction, red_move);
	vec2 blue_new_pos = combinePDM(player_blue.position, player_blue.direction, blue_move);
	
	if(!isOnBoard(red_new_pos))
	{
		red_move = 0;
		red_new_pos = player_red.position;
	}
	if(!isOnBoard(blue_new_pos))
	{
		blue_move = 0;
		blue_new_pos = player_blue.position;
	}
	if(red_move == 0 && blue_move == 0)
	{
		return;
	}
	
	// Check for collision
	if(red_new_pos == blue_new_pos && (red_move == 0 || blue_move == 0))
	{
		return;
	}
	
	if(red_new_pos == blue_new_pos && red_move != 0 && blue_move != 0)
	{
		if(player_red.health > player_blue.health)
		{
			player_red.position = red_new_pos;
			return;
		}
		else if(player_red.health < player_blue.health)
		{
			player_blue.position = blue_new_pos;
			return;
		}
		else
		{
			return;
		}
	}
	
	bool red_facing_blue = player_blue.position == combinePDM(player_red.position, player_red.direction, 1);
	bool blue_facing_red = player_red.position == combinePDM(player_blue.position, player_blue.direction, 1);
	
	// pressure rule
	if(red_facing_blue && !blue_facing_red && red_move == 0)
	{
		const auto not_allowed_location_1 = combinePDM(player_red.position, player_red.direction, 3);
		const auto not_allowed_location_2 = combinePDM(player_red.position, player_red.direction, 7);
		if(blue_new_pos == not_allowed_location_1 || blue_new_pos == not_allowed_location_2)
		{
			blue_new_pos = player_blue.position;
		}
	}
	if(blue_facing_red && !red_facing_blue && blue_move == 0)
	{
		const auto not_allowed_location_1 = combinePDM(player_blue.position, player_blue.direction, 3);
		const auto not_allowed_location_2 = combinePDM(player_blue.position, player_blue.direction, 7);
		if(red_new_pos == not_allowed_location_1 || red_new_pos == not_allowed_location_2)
		{
			red_new_pos = player_red.position;
		}
	}

	player_blue.position = blue_new_pos;
	player_red.position = red_new_pos;
	
	// Interception Not Coded
}

void Board::playout_action(const BaseCard* red_card, const BaseCard* blue_card)
{
	//Cooldown
	if(red_card->isCooldown())
	{
		player_red.not_usable_card.push_back(red_card->getName());
	}
	if(blue_card->isCooldown())
	{
		player_blue.not_usable_card.push_back(blue_card->getName());
	}
	
	// Movement
	makeValidMove(red_card->getMove(), blue_card->getMove());
	
	// Attack Damage
	auto is_hit = [](const Player& attacker, const Player& defender, const std::vector<int>& attack_targets) 
	{
		for (int attack_target : attack_targets)
		{
			if (defender.position == combinePDM(attacker.position, attacker.direction, attack_target))
			{
				return true;
			}
		}
		return false;
	};
	
	bool blue_hits_red = is_hit(player_blue, player_red, blue_card->getAttackTarget());
	bool red_hits_blue = is_hit(player_red, player_blue, red_card->getAttackTarget());
	//Dodge
	if(red_card->dodges_all_attacks())
	{
		blue_hits_red = false;
	}
	if(blue_card->dodges_all_attacks())
	{
		red_hits_blue = false;
	}
	
	//Counter
	if(red_card->attack_is_counter() != blue_card->attack_is_counter())
	{
		if(red_card->attack_is_counter())
		{
			blue_hits_red = false;
		}
		if(blue_card->attack_is_counter())
		{
			red_hits_blue = false;
		}
	}
	
	//Daze
	if(red_hits_blue && red_card->daze() && !blue_card->avoids_daze())
	{
		player_blue.is_dazed = true;
	}
	if(blue_hits_red && blue_card->daze() && !red_card->avoids_daze())
	{
		player_red.is_dazed = true;
	}
	
	// Deal Damage
	if(red_hits_blue)
	{
		player_blue.health -= blue_card->getDamageModification(red_card->getDamage());
	}
	if(blue_hits_red)
	{
		player_red.health -= red_card->getDamageModification(blue_card->getDamage());
	}
	
	// Attack Move
	red_attack_move_options = LookUpBasicCards(red_card->getAttackMove());
	blue_attack_move_options = LookUpBasicCards(blue_card->getAttackMove());
	
	// Rotate
	player_red.direction = (player_red.direction + red_card->getRotate() + 8) % 8;
	player_blue.direction = (player_blue.direction + blue_card->getRotate() + 8) % 8;
}

std::vector<std::vector<BaseCard*>> Board::LookUpBasicCards(std::vector<std::string> card_names)
{
	std::vector<std::vector<BaseCard*>> result;
	for(auto name : card_names)
	{
		result.push_back({find_card(name)});
	}
	return result;
}

int Board::game_winner(bool isPlayerRed )
{
	int winner = 0;
	
	if(player_red.health > 0 && player_blue.health > 0)
	{
		if(player_red.round_wins > player_blue.round_wins)
		{
			winner = 1;
		}
		if(player_red.round_wins < player_blue.round_wins)
		{
			winner =  -1;
		}
	}
	
	if(player_blue.health <= 0) 
	{
		winner = 1;
	}
	if(player_red.health <= 0)
	{
		winner =  -1;
	}
	if(!isPlayerRed)
	{
		winner = winner * (-1);
	}
	return winner;
}

void Board::printGrid() {
	
	auto getDirection = [](int value, bool isPlayerRed) -> std::string {
		return DIRECTION_LOOKUP[value][isPlayerRed];
	};
	
	std::cout << "Turn " << turn_count << std::endl << "eval: " << eval() << std::endl;
	std::cout << "+" << std::string(11, '-') << "+" << std::endl;
	for (int y = 4; y >= 0; --y) 
	{
		std::cout << "| ";
		for (int x = 0; x < 5; ++x) 
		{
			if (x == player_red.position.x && y == player_red.position.y)
			std::cout << getDirection(player_red.direction, true);
			else if (x == player_blue.position.x && y == player_blue.position.y)
			std::cout << getDirection(player_blue.direction, false);
			else
			std::cout << ".";
			std::cout << " ";
		}
		std::cout << "|";
		if(y==3) 
		std::cout << getDirection(player_red.direction, true) << "1♥: " << player_red.health;
		if(y==2) 
		std::cout << getDirection(player_blue.direction, false) << "2♥: " << player_blue.health;
		std::cout << "\n";
	}
	std::cout << "+" << std::string(11, '-') << "+" << std::endl;
	if(getDirection(player_red.direction, true).size() > 3)
	{
		std::cout << "error" << getDirection(player_red.direction, true) << std::endl;
	}
	if(getDirection(player_blue.direction, false).size() > 3)
	{
		std::cout << "error" << getDirection(player_blue.direction, false) << std::endl;
	}
}