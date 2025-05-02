#include <fstream>
#include <map>
#include <vector>
#include <iomanip> // For formatting output
#include <sstream> // For CSV formatting

#include "Board.h"
#include "main.h"

int play_single_game(Player player1, Player player2, Bot& bot)
{
	Board game1(player1, player2);
	while(!game1.is_game_over())
	{
		auto player1_action = bot.find_best_move(game1, true);
		auto player2_action = bot.find_best_move(game1, false);
		
		game1.submit_turn_action(player1_action, true);
		game1.submit_turn_action(player2_action, false);
	}
	
	return game1.game_winner();
}

void run_tournament(const std::vector<Player>& players, Bot& bot, int repeat_matches, const std::string& output_file) {
    std::map<std::string, int> player_scores; // Map to store player scores
    std::map<std::string, std::map<std::string, std::tuple<int, int, int>>> results_table; // Nested map for win/tie/loss table

    // Initialize scores and results table
    for (const auto& player : players) {
        player_scores[player.name] = 0;
        for (const auto& opponent : players) {
            results_table[player.name][opponent.name] = {0, 0, 0}; // {wins, ties, losses}
        }
    }

    // Run the tournament
    for (size_t i = 0; i < players.size(); ++i) {
        for (size_t j = 0; j < players.size(); ++j) {
            for (int repeat = 0; repeat < repeat_matches; ++repeat) {
                Player player1 = players[i];
                Player player2 = players[j];

                // Play the game
                int winner = play_single_game(player1, player2, bot);

                // Update scores and results table
                if (winner == 1) {
                    player_scores[player1.name] += 3; // Win = 3 points
                    std::get<0>(results_table[player1.name][player2.name])++; // Increment wins
                    std::get<2>(results_table[player2.name][player1.name])++; // Increment losses
                } else if (winner == -1) {
                    player_scores[player2.name] += 3; // Win = 3 points
                    std::get<0>(results_table[player2.name][player1.name])++; // Increment wins
                    std::get<2>(results_table[player1.name][player2.name])++; // Increment losses
                } else if (winner == 0) {
                    player_scores[player1.name] += 1; // Tie = 1 point each
                    player_scores[player2.name] += 1;
                    std::get<1>(results_table[player1.name][player2.name])++; // Increment ties
                    std::get<1>(results_table[player2.name][player1.name])++; // Increment ties
                }
            }
        }
    }

    // Write results to CSV file
    std::ofstream file(output_file);
    if (!file) {
        throw std::runtime_error("Failed to open output file: " + output_file);
    }

    // Write CSV header
    file << "#";
    for (const auto& player : players) {
        file << "," << player.name;
    }
    file << "\n";

    // Write CSV rows
    for (const auto& player : players) {
        file << player.name;
        for (const auto& opponent : players) {
            const auto& [wins, ties, losses] = results_table[player.name][opponent.name];
            file << "," << wins << "/" << ties << "/" << losses;
        }
        file << "\n";
    }

    // Sort players by score
    std::vector<std::pair<std::string, int>> rankings(player_scores.begin(), player_scores.end());
    std::sort(rankings.begin(), rankings.end(), [](const auto& a, const auto& b) {
        return b.second > a.second; // Sort in descending order of score
    });

    // Print final rankings to screen and file
    std::cout << "\nFinal Rankings:\n";
    std::cout << "===============\n";
    for (size_t rank = 0; rank < rankings.size(); ++rank) {
        std::cout << rank + 1 << ". " << rankings[rank].first << " - " << rankings[rank].second << " points\n";
    }

    file << "\nFinal Rankings:\n";
    file << "===============\n";
    for (size_t rank = 0; rank < rankings.size(); ++rank) {
        file << rank + 1 << ". " << rankings[rank].first << " - " << rankings[rank].second << " points\n";
    }

    file.close();
}