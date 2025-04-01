#include <iostream>
#include <vector>
#include <string>

#include "constants.h"
#include "main.h"


std::string getDirection(int value, bool isPlayer1) {
	return DIRECTION_LOOKUP[value][isPlayer1];
}

int diagonalConversion(int value) {
	// function is doing this 
	// return (value == 0) ? 0 : (value <= 8) ? (value % 8) + 1 : value + 2;

	static constexpr int lookup[] = {0, 2, 3, 4, 5, 6, 7, 8, 1, 11, 12, 13, 14};
	return lookup[value];
}

int makeMovement_old(int position, int direction, int movementIndex)
{
	if (direction%2)
		movementIndex = diagonalConversion(movementIndex);

	return position + MOVEMENT_LOOPUP[movementIndex][direction/2];
}

void Board::play_single_card(Card p1Card, Card p2Card) {
	player_1.position = makeMovement(player_1.position, player_1.direction, p1Card.move);
	player_2.position = makeMovement(player_2.position, player_2.direction, p2Card.move);

	for (const int &attack : p1Card.attack) {
		if (player_2.position == makeMovement(player_1.position, player_1.direction, attack))

	}

	if(makeMovement(player_1.position, player_1.direction, p1Card.move))
}

void Board::printGrid() {
	std::vector<std::string> arr(25, ".");
	arr[player_1.position] = getDirection(player_1.direction, true);
	arr[player_2.position] = getDirection(player_2.direction, false);

	std::cout << "+-----------+\n";
	for (int i = 4; i >= 0; --i) {  // Iterate from bottom to top
		std::cout << "| ";
		for (int j = 0; j < 5; ++j) {
			std::cout << arr[i * 5 + j] << " ";
		}
		std::cout << "|";
		if(i==3) 
			std::cout << "1♥: 15";
		if(i==2) 
			std::cout << "2♥: 15";
		std::cout << "\n";
	}
	std::cout << "+-----------+\n";
}



int main() {

	Player alice(15, 6, 1, {}, 0);
	Player bob(15, 18, 5, {}, 0);

	std::vector<Card> full_deck = {
        Card("Rotate", true, 0, {}),
        Card("Move_Forwards", false, 1, {}),
        Card("Move_Forwards-Right", false, 2, {}),
        Card("Move_Right", false, 3, {}),
        Card("Move_Backwards-Right", false, 4, {}),
        Card("Move_Backwards", false, 5, {}),
        Card("Move_Backwards-Left", false, 6, {}),
        Card("Move_Left", false, 7, {}),
        Card("Move_Forwards-Left", false, 8, {}),
        Card("Attack_Forwards", false, 0, {1}),
        Card("Attack_Left", false, 0, {8, 1}),
        Card("Attack_Right", false, 0, {1, 2}),
        Card("Attack_All", false, 0, {8, 1, 2}),
        Card("Attack_Far-Left", false, 0, {10, 11}),
        Card("Attack_Far-Right", false, 0, {9, 10}),
        Card("Attack_Far-All", false, 0, {9, 10, 11}),
        Card("Attack_Double-Forwards", false, 0, {1, 10}),
        Card("Attack_Double-Left", false, 0, {2, 12})
    };


	Board game1(alice,bob);
	game1.printGrid();
	game1.play_single_card(full_deck[3], full_deck[6]);
	game1.play_single_card(full_deck[3], full_deck[6]);
	game1.printGrid();

	return 0;
}
