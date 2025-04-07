#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <chrono>

#include "main.h"
#include "MCTSBot.h"

void play_game(Board& game1, Bot& bot1, Bot& bot2)
{
	while(!game1.is_game_over())
	{
		game1.printGrid();
		auto timeA = std::chrono::high_resolution_clock::now();

		auto bot1_action = bot1.find_best_move(game1, true);
		auto timeB = std::chrono::high_resolution_clock::now();

		auto bot2_action = bot2.find_best_move(game1, false);
		auto timeC = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> duration_bot1 = timeB - timeA;
		std::chrono::duration<double> duration_bot2 = timeC - timeB;

		std::cout <<  "Bot1 " << duration_bot1.count() << " s ";
		for( auto card : bot1_action)
		{
			std::cout << card.name << ", ";
		}
		std::cout << std::endl;
		std::cout << "Bot2 " << duration_bot2.count() << " s ";
		for( auto card : bot2_action)
		{
			std::cout << card.name << ", ";
		}
		std::cout << std::endl;
		game1.submit_turn_action(bot1_action, true);
		game1.submit_turn_action(bot2_action, false);
		// game1.play_action(bot1_action, bot2_action);
	}
	if(game1.game_winner() == 1)
	{
		std::cout << "BOT1 Wins" << std::endl;
	}
	else if(game1.game_winner() == -1)
	{
		std::cout << "BOT2 Wins" << std::endl;
	}
	else if(game1.game_winner() == 0)
	{
		std::cout << "Tied" << std::endl;
	}
}


int main() {
	
	std::vector<Action> full_deck = {
		Action("AA.", "Rotate_45_clockwise", 1, 0, {}),
		Action("AB.", "Rotate_45_anticlockwise", -1, 0, {}),
		Action("AC.", "Move_Forwards", 0, 1, {}),
		Action("AD.", "Move_Forwards_Right", 0, 2, {}),
		Action("AE.", "Move_Right", 0, 3, {}),
		Action("AF.", "Move_Backwards_Right", 0, 4, {}),
		Action("AG.", "Move_Backwards", 0, 5, {}),
		Action("AH.", "Move_Backwards_Left", 0, 6, {}),
		Action("AI.", "Move_Left", 0, 7, {}),
		Action("AJ.", "Move_Forwards_Left", 0, 8, {}),
		Action("AK.", "Attack_Forwards", 0, 0, {1}, 1),
		Action("AL.", "Attack_Left", 0, 0, {8, 1}, 1),
		Action("AM.", "Attack_Right", 0, 0, {1, 2}, 1),
		Action("AN.", "Attack_All", 0, 0, {8, 1, 2}, 1),
		Action("AO.", "Attack_Far_Left", 0, 0, {10, 11}, 1),
		Action("AP.", "Attack_Far_Right", 0, 0, {9, 10}, 1),
		Action("AQ.", "Attack_Far_All", 0, 0, {9, 10, 11}, 1),
		Action("AR.", "Attack_Double_Forwards", 0, 0, {1, 10}, 1),
		Action("AS.", "Attack_Double_Left", 0, 0, {2, 12}, 1)
	};
	
	std::vector<Action> small_deck = {
		Action("AT.", "Rotate_45_clockwise", 1, 0, {}),
		Action("AU.", "Rotate_45_anticlockwise", -1, 0, {}),
		Action("AV.", "Attack_ALL", 0, 0, {1, 2, 8, 9, 10, 11}, 2),
		Action("AW.", "Move_Forwards", 0, 1, {}),
		Action("AY.", "Move_Backwards", 0, 5, {})
	};
	std::vector<Action> only_rotate_deck = {
		Action("AT.", "Rotate_45_clockwise", 1, 0, {}),
		Action("AU.", "Rotate_45_anticlockwise", -1, 0, {}),
		Action("AY.", "Rotate_45_anticlockwise", 2, 0, {}),
		Action("AZ.", "Rotate_45_anticlockwise", -2, 0, {}),
	};


	Random bot1, bot2;
	BestCurrent bot3;
	MCTSBot bot4;
	Player alice(15, {1,1}, 1, only_rotate_deck);
	Player bob(15, {3,3}, 5, small_deck);

	Board game1(alice,bob);
	// game1.printGrid();
	// game1.play_action({small_deck[2]}, {});
	// game1.printGrid();
	// game1.play_action({small_deck[2]}, {});
	// game1.printGrid();
	
	play_game(game1, bot1, bot2);

	return 0;
}

