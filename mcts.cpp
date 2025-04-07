// #include <cmath>
// #include <unordered_map>
// #include <random>

// #include "main.h"

// struct MCTSNode
// {
//     Board board;
//     std::vector<std::vector<Action>> pRed_moves;
//     std::vector<std::vector<Action>> pBlue_moves;
//     std::unordered_map<std::string, int> wins, visits;
//     bool isPlayerRed;
// };

// // UCB1 formula for node selection
// float UCT(float wins, float visits, float totalVisits)
// {
//     if (visits == 0)
//         return std::numeric_limits<float>::infinity(); // Encourage exploration
//     return (wins / visits) + 1.41 * std::sqrt(std::log(totalVisits) / visits);
// }

// // Random playout (simulates a game to completion)
// float simulate(Board board)
// {
//     std::random_device rd;
//     std::mt19937 gen(rd());

//     while (!board.is_game_over()) // Assumes Board has an is_game_over() function
//     {
//         auto pRed_moves = board.player_red.get_all_moves();
//         auto pBlue_moves = board.player_blue.get_all_moves();

//         std::uniform_int_distribution<int> dist1(0, pRed_moves.size() - 1);
//         std::uniform_int_distribution<int> dist2(0, pBlue_moves.size() - 1);

//         board.play_action(pRed_moves[dist1(gen)], pBlue_moves[dist2(gen)]);
//     }

//     return board.game_winner(); // Final evaluation
// }

// std::vector<Action> mcts(Board board, bool isPlayerRed, int simulations)
// {
//     MCTSNode root{board, board.player_red.get_all_moves(), board.player_blue.get_all_moves(), {},{}, isPlayerRed};
    
//     for (int i = 0; i < simulations; i++)
//     {
//         MCTSNode* node = &root;
//         Board currentBoard = board;

//         // Selection
//         while (!currentBoard.is_game_over() && !node->pRed_moves.empty() && !node->pBlue_moves.empty())
//         {
//             float bestValue = -std::numeric_limits<float>::infinity();
//             std::vector<Action> bestMove;

//             for (const auto& move : (isPlayerRed ? node->pRed_moves : node->pBlue_moves))
//             {
//                 std::string moveKey = std::to_string(i); // Hash move uniquely
//                 float uctValue = UCT(node->wins[moveKey], node->visits[moveKey], i + 1);

//                 if (uctValue > bestValue)
//                 {
//                     bestValue = uctValue;
//                     bestMove = move;
//                 }
//             }

//             currentBoard.play_action(isPlayerRed ? bestMove : std::vector<Action>{}, 
//                                      !isPlayerRed ? bestMove : std::vector<Action>{});
//         }

//         // Expansion
//         auto pRed_moves = currentBoard.player_red.get_all_moves();
//         auto pBlue_moves = currentBoard.player_blue.get_all_moves();
//         if (!pRed_moves.empty() && !pBlue_moves.empty())
//         {
//             std::random_device rd;
//             std::mt19937 gen(rd());

//             std::uniform_int_distribution<int> dist1(0, pRed_moves.size() - 1);
//             std::uniform_int_distribution<int> dist2(0, pBlue_moves.size() - 1);

//             std::vector<Action> pRed_move = pRed_moves[dist1(gen)];
//             std::vector<Action> pBlue_move = pBlue_moves[dist2(gen)];

//             currentBoard.play_action(pRed_move, pBlue_move);
//         }

//         // Simulation
//         float result = simulate(currentBoard);

//         // Backpropagation
//         std::string moveKey = std::to_string(i); // Hash move uniquely
//         root.wins[moveKey] += result;
//         root.visits[moveKey]++;
//     }

//     // Choose best move based on visits
//     std::string bestMoveKey;
//     int maxVisits = 0;
//     for (const auto& [key, count] : root.visits)
//     {
//         if (count > maxVisits)
//         {
//             maxVisits = count;
//             bestMoveKey = key;
//         }
//     }

//     return root.pRed_moves[std::stoi(bestMoveKey)]; // Return best move found
// }


// std::vector<Action> mctsbot_old::find_best_move(const Board& board, bool isPlayerRed)
// {
// 	return mcts(board, isPlayerRed, 1000);
// }