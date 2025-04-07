#pragma once
#include <string>

enum class TokenType {
	Not_Submitted,
	None,
	Burst,
	React
};

enum TurnOrder {
	Token = 0,
	Reveal,
	Card
}; 

struct vec2 {
	int x = 0, y = 0; // Default initialization

	// Constructor
	vec2(int x_val = 0, int y_val = 0) : x(x_val), y(y_val) {}

	// Overload + operator
	vec2 operator+(const vec2& other) const {
		return vec2(x + other.x, y + other.y);
	}
	// Overload == operator for comparison
	bool operator==(const vec2& other) const {
		return x == other.x && y == other.y;
	}
};


static constexpr int COORD_INDEX_LOOKUP[5][5] = {
		{20, 21, 22, 23, 24},
		{15, 16, 17, 18, 19},
		{10, 11, 12, 13, 14},
		{5, 6, 7, 8, 9},
		{0, 1, 2, 3, 4}
	};

static constexpr int INDEX_COORD_LOOKUP[25][2] = {
	{4, 0},
	{4, 1},
	{4, 2},
	{4, 3},
	{4, 4},
	{3, 0},
	{3, 1},
	{3, 2},
	{3, 3},
	{3, 4},
	{2, 0},
	{2, 1},
	{2, 2},
	{2, 3},
	{2, 4},
	{1, 0},
	{1, 1},
	{1, 2},
	{1, 3},
	{1, 4},
	{0, 0},
	{0, 1},
	{0, 2},
	{0, 3},
	{0, 4}
};

const std::string DIRECTION_LOOKUP[8][2] = {
	{"↑" , "⇧"},
	{"↗" , "⬀"},
	{"→" , "⇨"},
	{"↘" , "⬂"},
	{"↓" , "⇩"},
	{"↙" , "⬃"},
	{"←" , "⇦"},
	{"↖" , "⬁"}
};

static constexpr int MOVEMENT_LOOPUP[15][4] = {
	{  0,   0,   0,   0 },
	{ +5,  +1,  -5,  -1 },
	{ +6,  -4,  -6,  +4 },
	{ +1,  -5,  -1,  +5 },
	{ -4,  -6,  +4,  +6 },
	{ -5,  -1,  +5,  +1 },
	{ -6,  +4,  +6,  -4 },
	{ -1,  +5,  +1,  -5 },
	{ +4,  +6,  -4,  -6 },
	{ +9,  +7,  -9,  -7 },
	{ +10, +2, -10,  -2 },
	{ +11, -3, -11,  +3 },
	{ +12, -8, -12,  +8 },
	{ +7,  -9,  -7,  +9 },
	{ +2, -10,  -2, +10 }
};

const vec2 ONE_SPACE_MOVE[8] = {
	{0, 1}, {1, 1}, {1, 0}, {1, -1},
	{0, -1}, {-1, -1}, {-1, 0}, {-1, 1}
};

const vec2 TWO_SPACE_MOVE[16] = {
	{0,2},
	{1,2},
	{2,2},
	{2,1},
	{2,0},
	{2,-1},
	{2,-2},
	{1,-2},
	{0,-2},
	{-1,-2},
	{-2,-2},
	{-2,-1},
	{-2,0},
	{-2,1},
	{-2,2},
	{-1,2}
};