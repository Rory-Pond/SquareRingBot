#pragma once
#include <vector>
#include <string>

class Board;
class BaseCard;
class Player;

class Bot 
{
	public:
		virtual std::vector<BaseCard*> find_best_move(const Board& board, bool isPlayerRed) = 0;
		std::string name;
};

void run_tournament(const std::vector<Player>& players, Bot& bot, int repeat_matches, const std::string& output_file);