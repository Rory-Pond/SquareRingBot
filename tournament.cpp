#include <fstream>
#include <sstream>
#include <iomanip>

void run_round_robin_tournament_to_file(
    const std::vector<Player>& players,
    const std::vector<Bot*>& bots,
    int games_per_pair,
    const std::string& output_filename)
{
    int num_players = players.size();
    std::map<std::string, std::map<std::string, MatchResult>> results;

    for (int i = 0; i < num_players; ++i) {
        for (int j = i + 1; j < num_players; ++j) {
            Player player1 = players[i];
            Player player2 = players[j];
            Bot* bot1 = bots[i];
            Bot* bot2 = bots[j];

            for (int g = 0; g < games_per_pair; ++g) {
                bool swap = g % 2 == 1;
                Player red = swap ? player2 : player1;
                Player blue = swap ? player1 : player2;
                Bot* red_bot = swap ? bot2 : bot1;
                Bot* blue_bot = swap ? bot1 : bot2;

                Board board(red, blue);

                while (!board.is_game_over()) {
                    auto red_move = red_bot->find_best_move(board, true);
                    auto blue_move = blue_bot->find_best_move(board, false);
                    board.play_moves(red_move, blue_move);  // Implement this
                }

                int outcome = board.game_winner();
                std::string red_name = red.get_name();
                std::string blue_name = blue.get_name();

                if (outcome == 1) {
                    results[red_name][blue_name].wins++;
                    results[blue_name][red_name].losses++;
                } else if (outcome == -1) {
                    results[red_name][blue_name].losses++;
                    results[blue_name][red_name].wins++;
                } else {
                    results[red_name][blue_name].draws++;
                    results[blue_name][red_name].draws++;
                }
            }
        }
    }

    // Write results to file
    std::ofstream out(output_filename);
    if (!out.is_open()) {
        std::cerr << "Failed to open output file: " << output_filename << std::endl;
        return;
    }

    // CSV Header
    out << "Player";
    for (const auto& p : players) {
        out << "," << p.get_name();
    }
    out << "\n";

    for (const auto& p1 : players) {
        out << p1.get_name();
        for (const auto& p2 : players) {
            if (p1.get_name() == p2.get_name()) {
                out << ",X";
            } else {
                const auto& res = results[p1.get_name()][p2.get_name()];
                out << "," << res.wins << "W " << res.losses << "L " << res.draws << "D";
            }
        }
        out << "\n";
    }

    out.close();
    std::cout << "Tournament results written to " << output_filename << std::endl;
}
