import math
import numpy as np

class Player:
    def __init__(self, position, direction):
        self.pos = position
        self.dirc = direction

def player1icon(dirc):
    ca

player1icon = {
	0: "↑",
	1: "↗",
	2: "→",
	3: "↘",
	4: "↓",
	5: "↙",
	6: "←",
	7: "↖",
}
player2icon = {
	0: "⇧",
	1: "⬀",
	2: "⇨",
	3: "⬂",
	4: "⇩",
	5: "⬃",
	6: "⇦",
	7: "⬁",
}

player1 = 7
dirc = 2

board = ["."] * 25
board[player1] = player1icon.get(dirc)

# print(board)
# print('+-----------+\n| . . ⬁ . . |\n| . . . ↗ . |\n| . . . . . |\n| . . . . . |\n| . . . . . |\n+-----------+')


def print_grid(arr):
    if len(arr) != 25:
        raise ValueError("Array must have exactly 25 elements.")

    print("+-----------+")
    for i in range(4, -1, -1):  # Start from row index 4 (bottom-up)
        print("|", " ".join(arr[i * 5:(i + 1) * 5]), "|")
    print("+-----------+")


print_grid(board)