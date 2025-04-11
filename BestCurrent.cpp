
#include <algorithm>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <vector>

#include "constants.h"
#include "main.h"

// std::vector<Action> BestCurrent::find_best_move(const Board& board, bool isPlayerRed)
// {
// 	std::vector<Action> best_move; 
// 	auto possibleMoves = isPlayerRed ? board.player_red.get_all_moves() :  board.player_blue.get_all_moves();
// 	float best_eval = -1000;
// 	for (const auto& move : possibleMoves) 
// 	{
// 		Board tempBoard = board;
// 		std::vector<Action> null_move = {Action(), Action()};
// 		std::vector<Action> pRed_move = isPlayerRed ? move : null_move;
// 		std::vector<Action> pBlue_move = isPlayerRed ? null_move : move;
// 		tempBoard.play_action(pRed_move, pBlue_move);
// 		auto new_eval = tempBoard.eval(isPlayerRed);
// 		if(new_eval > best_eval)
// 		{
// 			best_eval = new_eval;
// 			best_move = isPlayerRed ? pRed_move : pBlue_move;
// 		}
// 	}
// 	return best_move;

// }
