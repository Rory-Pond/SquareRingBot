#include <iostream>
#include <vector>
#include <string>

#include "constants.h"
class Card
{
	private:
	public:
		std::string name;
		bool rotate;
		int move;
		std::vector<int> attack;
		Card(std::string card_name, bool r, int m, const std::vector<int>& a)
			: name(card_name), rotate(r), move(m), attack(a) {}
};

class Player 
{
	public:
		int health;
		vec2 position;
		int direction;
		std::vector<Card> deck;
		std::optional<int> last_used_card;
		int statusEffects;
		
		// Constructor
		Player(int h, int pos, int dir, const std::vector<int>& crds, int effects =0)
		: health(h), position(pos), direction(dir), cards(crds), statusEffects(effects) {}

		std::string character_sprite;
};

class Board
{
	private:
		Player player_1, player_2;
		int turn_count = 0;
	public:
		Board(Player p_1, Player p_2)
		:player_1(p_1), player_2(p_2){}

		void printGrid();

		void play_move(int p1CardIndex, int p2CardIndex);
};