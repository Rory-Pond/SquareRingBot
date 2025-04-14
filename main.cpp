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
}


int main() {
	Libary cardList;
	const std::vector<BaseCard*> Oscar_Deck = {
		cardList.getCard("Rotate_90"),
		cardList.getCard("Move_Forwards_Rotate"),
		cardList.getCard("Move_Right"),
		cardList.getCard("Move_Backwards"),
		cardList.getCard("Move_Left"),
		cardList.getCard("Move_Forwards_Left"),
		cardList.getCard("Cannonita_Oscar"),
		cardList.getCard("Cajones_Oscar"),
		cardList.getCard("Left_Haymaker_Oscar"),
		cardList.getCard("El_Nuke_Oscar")
	};
	
	const std::vector<BaseCard*> Inga_Deck = {
		cardList.getCard("Rotate_90"),
		cardList.getCard("Move_Forwards"),
		cardList.getCard("Move_Right"),
		cardList.getCard("Move_Backwards_Right_Rotate"),
		cardList.getCard("Move_Backwards"),
		cardList.getCard("Move_Left"),
		cardList.getCard("Move_Forwards_Left_Rotate"),
		cardList.getCard("Left_Jab_Inga"),
		cardList.getCard("Right_Hook_Inga"),
		cardList.getCard("Svallin_Inga"),
		cardList.getCard("Mjolnir_Inga")
	};
	
	
	// const std::vector<BaseCard*> Fredo_Deck = {
	// 	cardList.getCard("Rotate_90"),
	// 	cardList.getCard("Move_Forwards_Rotate"),
	// 	cardList.getCard("Move_Forwards_Right_Rotate"),
	// 	cardList.getCard("Move_Right"),
	// 	cardList.getCard("Move_Backwards"),
	// 	cardList.getCard("Move_Left"),
	// 	cardList.getCard("Move_Forwards_Left_Rotate"),
	// 	cardList.getCard("Left_Hook_Fredo"),
	// 	cardList.getCard("Right_Hook_Fredo"),
	// 	cardList.getCard("Grill_Guard_Fredo"),
	// 	cardList.getCard("Uppercut_Fredo")
	// };
	
	
	// const std::vector<BaseCard*> Andrey_Deck = {
	
	// 	cardList.getCard("Rotate_90"),
	// 	cardList.getCard("Move_Forwards"),
	// 	cardList.getCard("Move_Right_Rotate"),
	// 	cardList.getCard("Move_Backwards_Right"),
	// 	cardList.getCard("Move_Backwards"),
	// 	cardList.getCard("Move_Left_Rotate"),
	// 	cardList.getCard("Move_Forwards_Left"),
	// 	cardList.getCard("Collider_Andrey"),
	// 	cardList.getCard("Quantum_Jab_Andrey"),
	// 	cardList.getCard("Paradox_Andrey"),
	// 	cardList.getCard("Entangle_Andrey")
	// };
	// const std::vector<BaseCard*> Ellie_Deck = {
	// 	cardList.getCard("Rotate_90"),
	// 	cardList.getCard("Move_Forwards_Rotate"),
	// 	cardList.getCard("Move_Forwards_Right_Rotate"),
	// 	cardList.getCard("Move_Right_Rotate"),
	// 	cardList.getCard("Move_Backwards_Right_Rotate"),
	// 	cardList.getCard("Move_Backwards_Rotate"),
	// 	cardList.getCard("Move_Backwards_Left_Rotate"),
	// 	cardList.getCard("Move_Left_Rotate"),
	// 	cardList.getCard("Move_Forwards_Left_Rotate"),
	// 	cardList.getCard("Left_Jab_Ellie"),
	// 	cardList.getCard("Godiva_Strafe_Ellie"),
	// 	cardList.getCard("Groove_Ellie"),
	// 	cardList.getCard("Kook_Hook_Ellie")
	// };
	
	
	// const std::vector<BaseCard*> Awan_Deck = {
	// 	cardList.getCard("Rotate_90"),
	// 	cardList.getCard("Move_Forwards"),
	// 	cardList.getCard("Move_Right_Rotate"),
	// 	cardList.getCard("Move_Backwards_Right"),
	// 	cardList.getCard("Move_Backwards"),
	// 	cardList.getCard("Move_Left"),
	// 	cardList.getCard("Move_Forwards_Left_Rotate"),
	// 	cardList.getCard("Bolo_Punch_Awan"),
	// 	cardList.getCard("Flurry_Awan"),
	// 	cardList.getCard("Left_Jab_Awan"),
	// 	cardList.getCard("Rush_Guard_Awan")
	// };
	// const std::vector<BaseCard*> Roderik_Deck = {
	// 	cardList.getCard("Rotate_90"),
	// 	cardList.getCard("Move_Forwards"),
	// 	cardList.getCard("Move_Right"),
	// 	cardList.getCard("Move_Backwards_Right"),
	// 	cardList.getCard("Move_Backwards_Rotate"),
	// 	cardList.getCard("Move_Backwards_Left"),
	// 	cardList.getCard("Move_Left_Rotate"),
	// 	cardList.getCard("Check_Hook_Roderik"),
	// 	cardList.getCard("Cobra_Cross_Roderik"),
	// 	cardList.getCard("Left_Jab_Roderik"),
	// 	cardList.getCard("Coil_Up_Roderik")
	// };
	
	// const std::vector<BaseCard*> Nigel_Deck = {
	// 	cardList.getCard("Rotate_90"),
	// 	cardList.getCard("Move_Forwards_Rotate_Damage_Daze"),
	// 	cardList.getCard("Move_Forwards_Right"),
	// 	cardList.getCard("Move_Right"),
	// 	cardList.getCard("Move_Backwards_Rotate_Counter"),
	// 	cardList.getCard("Move_Backwards_Left_Rotate"),
	// 	cardList.getCard("Move_Left_Rotate"),
	// 	cardList.getCard("Straight_Nigel"),
	// 	cardList.getCard("Wallop_Nigel"),
	// 	cardList.getCard("Sway_Nigel"),
	// 	cardList.getCard("Right_Jolt_Nigel")
	// };


	// const std::vector<BaseCard*> tes_Deck = {
	// 	cardList.getCard("Rotate_90"),
	// 	cardList.getCard("Move_Forwards_Rotate"),
	// 	cardList.getCard("Move_Right"),
	// 	cardList.getCard("Move_Backwards"),
	// 	cardList.getCard("Move_Left"),
	// 	cardList.getCard("Move_Forwards_Left_Rotate"),
	// 	cardList.getCard("Left_Jab_Inga"),
	// 	cardList.getCard("Right_Hook_Inga"),
	// 	cardList.getCard("Svallin_Inga"),
	// 	cardList.getCard("Mjolnir_Inga")
	// };
	
	// std::cout << tes_Deck[1]->getName() << std::endl;
	// std::cout << tes_Deck[1]->getMove() << std::endl;
	
	// BaseCard* test = cardList.getCard("Move_Right");
	// std::cout << test->getName() << std::endl;
	// std::cout << test->getMove() << std::endl;
	
	// small_deck.push_back(Uppercut());
	
	RandomBot bot1, bot2;
	GreedyBot bot3;
	RandomSumBot bot4;
	TimedRandomSumBot bot5, bot6;
	RandmSumParrallelBot bot7;
	MCTSBot bot8;
	// // BestCurrent bot3;
	// // MCTSBot bot4;
	Player alice(15, {1,1}, 1, Oscar_Deck);
	Player bob(15, {3,3}, 5, Inga_Deck);
	
	Board game1(cardList, alice, bob);

	// game1.printGrid();
	// std::cout << game1.player_red.position.x << " " << game1.player_red.position.y << std::endl;
	// std::cout << game1.player_blue.position.x << " " << game1.player_blue.position.y << std::endl;
	// game1.submit_turn_action({{cardList.getCard("Token_None")}}, true);
	// game1.submit_turn_action({{cardList.getCard("Token_None")}}, false);
	// game1.submit_turn_action({cardList.getCard("Move_Left"), cardList.getCard("Move_Left")}, true);
	// game1.submit_turn_action({cardList.getCard("Move_Forwards"), cardList.getCard("Move_Forwards")}, false);
	// std::cout << game1.player_red.position.x << " " << game1.player_red.position.y << std::endl;
	// std::cout << game1.player_blue.position.x << " " << game1.player_blue.position.y << std::endl;
	// game1.printGrid();
	
	play_game(game1, bot6, bot8);
	
	return 0;
}

