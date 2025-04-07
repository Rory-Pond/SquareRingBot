#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
#include <random>

#include "main.h"

class Random: public Bot
{
	public:
	std::vector<Action> find_best_move(const Board& board, bool isPlayerRed)
	{
		auto moves = board.get_all_moves(isPlayerRed);
		if (moves.empty()) return {};
		
		static std::mt19937 rng(42); 
		std::uniform_int_distribution<int> distribution(0, moves.size() - 1);
	
		return moves[distribution(rng)];
	};
};

