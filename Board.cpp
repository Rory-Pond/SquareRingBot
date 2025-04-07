#include <iostream>
#include <vector>
#include <string>
#include <limits>

#include "main.h"
#include "Random.h"

int chebyshev_distance(const vec2& a, const vec2& b) {
	return std::max(std::abs(b.x - a.x), std::abs(b.y - a.y));
}

float Board::eval(bool isPlayerRed)
{
	float distance = chebyshev_distance(player_red.position, player_blue.position)*0.1;
	float healthChange = isPlayerRed ? (player_red.health - player_blue.health ): (player_blue.health - player_red.health);
	return healthChange - distance;
}

void Board::play_action(std::vector<Action> pRed_cards, std::vector<Action> pBlue_cards)
{
	if(game_over)
	{
		return;
	}

	if(pRed_cards.size() > pBlue_cards.size())
	{
		pBlue_cards.push_back(Action());
	}
	if(pBlue_cards.size() > pRed_cards.size())
	{
		pRed_cards.push_back(Action());
	}
	std::random_device rd;
	std::mt19937 g(rd());
	if(player_red.is_dazed)
	{
		//shuffle cards
		std::shuffle(pRed_cards.begin(), pRed_cards.end(), g);
		player_red.is_dazed = false;
	}
	if(player_blue.is_dazed)
	{
		//shuffle cards
		std::shuffle(pBlue_cards.begin(), pBlue_cards.end(), g);
		player_blue.is_dazed = false;
	}

	for (int i = 0; i < pRed_cards.size(); i++)
	{
		playout_action(pRed_cards[i], pBlue_cards[i]);
		if(player_red.health <= 0 || player_blue.health <= 0)
		{
			game_over = true;
			return;
		}
	}
	
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

		if(turn_count == 18)
		{
			game_over = true;
			return;
		}
		player_red.reset_round(true);
		player_blue.reset_round(false);
	}
}

void Board::submit_turn_action(const std::vector<Action>& action, bool isPlayerRed)
{
	switch (turn_order) 
	{
		case Token:
			if(isPlayerRed)
			{
				red_player_token = action[0].token;
			}
			else
			{
				blue_player_token = action[0].token;
			}
			if(red_player_token != TokenType::Not_Submitted && blue_player_token != TokenType::Not_Submitted)
			{
				resolveTokens();
				turn_order = static_cast<TurnOrder>((turn_order + 1) % 3);
			}
			break;
		case Reveal:
			if(isPlayerRed)
			{
				if(blue_player_token == TokenType::React)
				{
					player_red_current_action.push_back(action[0]);
				}
			}
			else
			{
				if(red_player_token == TokenType::React)
				{
					player_blue_current_action.push_back(action[0]);
				}
			}
			if(red_player_token == TokenType::React && !player_blue_current_action.empty() ||
			   blue_player_token == TokenType::React && !player_red_current_action.empty() || 
			   red_player_token != TokenType::React && blue_player_token != TokenType::React)
			{
				turn_order = static_cast<TurnOrder>((turn_order + 1) % 3);
			}
			break;

		case Card:
			if(isPlayerRed)
			{
				player_red_current_action.insert(player_red_current_action.end(), action.begin(), action.end());
			}
			else
			{
				player_blue_current_action.insert(player_blue_current_action.end(), action.begin(), action.end());
			}
			ResolveTurn();
			turn_order = static_cast<TurnOrder>((turn_order + 1) % 3);
			break;
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
	play_action(player_red_current_action, player_blue_current_action);
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

std::vector<std::vector<Action>> Board::get_all_moves(bool isPlayerRed) const
{
	const TokenType self_token  = isPlayerRed ? red_player_token : blue_player_token;
	const TokenType other_token = isPlayerRed ? blue_player_token : red_player_token;

	int card_count = 2;
	if (other_token == TokenType::React) --card_count;
	if (self_token == TokenType::Burst)  ++card_count;

	switch (turn_order) {
		case Token:
		{
			if(isPlayerRed)
			{
				return player_red.get_all_token_moves();
			}
			else
			{
				return player_blue.get_all_token_moves();
			}
		}

		case Reveal:
		{
			if(isPlayerRed)
			{
				return (blue_player_token == TokenType::React)
					   ? player_red.get_all_actions_for_cards(1)
					   : std::vector<std::vector<Action>>{{Action()}};
			}
			else
			{
				return (red_player_token == TokenType::React)
					   ? player_blue.get_all_actions_for_cards(1)
					   : std::vector<std::vector<Action>>{{Action()}};
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

// int Board::whos_turn(bool isPlayerRed = true)
// {
// 	if(player_red_current_action.empty() && player_blue_current_action.empty())
// 	{
// 		if(player_red_react) return isPlayerRed ? 1 : -1;
// 		if(player_blue_react) return isPlayerRed ? -1 : 1;
// 		return 0;
// 	}
	
// 	if(player_red_current_action.empty())
// 		return isPlayerRed ? -1 : 1;
	
// 	if(player_blue_current_action.empty())
// 		return isPlayerRed ? 1 : -1;
	
// 	if(player_red_current_action.size() == 1)
// 		return isPlayerRed ? 1 : -1;
// }

vec2 combinePDM(vec2 pos, int dir, int move)
{
	auto value2 = positionFromIndex(move, dir);
	auto value = pos + positionFromIndex(move, dir);
	return value;
}

void Board::playout_action(const Action& pRedCard, const Action& pBlueCard)
{
	// std::cout << "pRed: " << pRedCard.name << "	pBlue: " << pBlueCard.name << std::endl;
	// Movement
	vec2 pRed_New = combinePDM(player_red.position, player_red.direction, pRedCard.move);
	vec2 pBlue_New = combinePDM(player_blue.position, player_blue.direction, pBlueCard.move);
	
	// Check Movement
	if(isOnBoard(pRed_New))
		player_red.position = pRed_New;
	if(isOnBoard(pBlue_New))
		player_blue.position = pBlue_New;
	//TODO Additional Move rules


	// Attack Damage
	bool player_red_hit = false;
	bool player_blue_hit = false;
	for(int attackInd : pRedCard.attack)
	{
		if(player_blue.position == combinePDM(player_red.position, player_red.direction, attackInd))
		{
			
			player_red_hit = true;
			player_blue.health = player_blue.health - pRedCard.damage;
			break;
		}
	}
	for(int attackInd : pBlueCard.attack)
	{
		if(player_red.position == combinePDM(player_blue.position, player_blue.direction, attackInd))
		{
			player_red.health = player_red.health - pBlueCard.damage;
			
			
		}
	}

	// Attack Move
	if(player_red_hit && !player_blue_hit)
	{
		vec2 pRed_New_A = combinePDM(player_red.position, player_red.direction, pRedCard.attack_move);
		if(isOnBoard(pRed_New_A))
			player_red.position = pRed_New_A;
		player_red.direction = (player_red.direction + pRedCard.attack_rotate) % 8;
	}
	if(player_blue_hit && !player_red_hit)
	{
		vec2 pBlue_New_A = combinePDM(player_blue.position, player_blue.direction, pBlueCard.attack_move);
		if(isOnBoard(pBlue_New_A))
			player_blue.position = pBlue_New_A;
		player_blue.direction = (player_blue.direction + pBlueCard.attack_rotate) % 8;
	}
	
	// Rotate
	player_red.direction = (player_red.direction + pRedCard.rotate + 8) % 8;
	player_blue.direction = (player_blue.direction + pBlueCard.rotate + 8) % 8;
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
	
	std::cout << "Turn " << turn_count << "eval: " << eval() << std::endl;
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