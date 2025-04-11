#pragma once
#include <iostream>
#include <vector>
#include <string>

#include "constants.h"
#include "cards.h"
#include "Board.h"
#include "libary.h"

class Bot 
{
	public:
		virtual std::vector<BaseCard*> find_best_move(const Board& board, bool isPlayerRed) = 0;
		std::string name;
};

class BestCurrent: public Bot
{
	public:
	std::vector<BaseCard*> find_best_move(const Board& board, bool isPlayerRed);
};

class MinMax: public Bot
{
	public:
	std::vector<BaseCard*> find_best_move(const Board& board, bool isPlayerRed);
	static size_t randomIndex(size_t size);
	static int minimax(
		Board& board, 
		const std::vector<BaseCard*>& move1, 
		const std::vector<BaseCard*>& move2, 
		int depth, bool isMaximizing, 
		int alpha, int beta);
};