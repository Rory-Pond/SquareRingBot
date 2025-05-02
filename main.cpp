#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <chrono>

#include "main.h"
#include "libary.h"
#include "MCTSBot.h"
#include "Random.h"
#include "Monte.h"

#include "CardLibrary.h"
#include "CardFactory.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>


int play_game(Board& game1, Bot& bot1, Bot& bot2)
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
			std::cout << card->getName() << ", ";
		}
		std::cout << std::endl;
		std::cout << "Bot2 " << duration_bot2.count() << " s ";
		for( auto card : bot2_action)
		{
			std::cout << card->getName() << ", ";
		}
		std::cout << std::endl;
		game1.submit_turn_action(bot1_action, true);
		game1.submit_turn_action(bot2_action, false);
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
	return game1.game_winner();
}


// int main() {
// 	CardFactoryFunctions::register_card_types();
// 	CardLibrary::instance().load_from_json("../cards-simple.json");
// 	auto players = CardFactoryFunctions::create_players_from_json("../cards-simple.json");
	
// 	const std::vector<BaseCard*> Oscar_Deck = {
// 		find_card("Rotate_90"),
// 		find_card("Move_Forwards_Rotate"),
// 		find_card("Move_Right"),
// 		find_card("Move_Backwards"),
// 		find_card("Move_Left"),
// 		find_card("Move_Forwards_Left"),
// 		find_card("Cannonita_Oscar"),
// 		find_card("Cajones_Oscar"),
// 		find_card("Left_Haymaker_Oscar"),
// 		find_card("El_Nuke_Oscar")
// 	};
	
// 	// std::cout << tes_Deck[1]->getName() << std::endl;
// 	// std::cout << tes_Deck[1]->getMove() << std::endl;
	
// 	// BaseCard* test = find_card("Move_Right");
// 	// std::cout << test->getName() << std::endl;
// 	// std::cout << test->getMove() << std::endl;
	
// 	// small_deck.push_back(Uppercut());
	
// 	RandomBot bot1, bot2;
// 	GreedyBot bot3;
// 	RandomSumBot bot4;
// 	TimedRandomSumBot bot5, bot6;
// 	RandmSumParrallelBot bot7;
// 	MCTSBot bot8;
// 	// // BestCurrent bot3;
// 	// // MCTSBot bot4;
// 	Player alice(15, {1,1}, 1, Oscar_Deck);
// 	Player bob(15, {3,3}, 5, Oscar_Deck);
	
// 	Board game1(alice, bob);

// 	// game1.printGrid();
// 	// std::cout << game1.player_red.position.x << " " << game1.player_red.position.y << std::endl;
// 	// std::cout << game1.player_blue.position.x << " " << game1.player_blue.position.y << std::endl;
// 	// game1.submit_turn_action({{find_card("Token_None")}}, true);
// 	// game1.submit_turn_action({{find_card("Token_None")}}, false);
// 	// game1.submit_turn_action({find_card("Move_Left"), find_card("Move_Left")}, true);
// 	// game1.submit_turn_action({find_card("Move_Forwards"), find_card("Move_Forwards")}, false);
// 	// std::cout << game1.player_red.position.x << " " << game1.player_red.position.y << std::endl;
// 	// std::cout << game1.player_blue.position.x << " " << game1.player_blue.position.y << std::endl;
// 	// game1.printGrid();
	
// 	play_game(game1, bot1, bot2);
	
// 	return 0;
// }



int main() {
    CardFactoryFunctions::register_card_types();
    // CardLibrary::instance().load_from_json("../cards-simple.json");
    CardLibrary::instance().load_from_json("../Cards.json");
    auto players = CardFactoryFunctions::create_players_from_json("../characters.json");

    RandomBot bot;
    int repeat_matches = 3;
    std::string output_file = "tournament_results.csv";

    run_tournament(players, bot, repeat_matches, output_file);

    return 0;
}