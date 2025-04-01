#include <vector>
#include <limits>
#include <random>

struct card; // Assuming card is a defined type

class MinimaxAI {
public:
    static std::pair<std::vector<card>, std::vector<card>> findBestMove(Board& board, 
                                                                        const std::vector<card>& player1Cards, 
                                                                        const std::vector<card>& player2Cards, 
                                                                        int depth) {
        int bestValue = std::numeric_limits<int>::min();
        std::vector<std::pair<std::vector<card>, std::vector<card>>> bestMoves;

        auto possibleMoves1 = generateCardPairs(player1Cards);
        auto possibleMoves2 = generateCardPairs(player2Cards);

        for (const auto& move1 : possibleMoves1) {
            for (const auto& move2 : possibleMoves2) {
                Board tempBoard = board; // Clone the board
                int moveValue = minimax(tempBoard, move1, move2, depth, false, 
                                        std::numeric_limits<int>::min(), 
                                        std::numeric_limits<int>::max());
                
                if (moveValue > bestValue) {
                    bestValue = moveValue;
                    bestMoves.clear();
                    bestMoves.push_back({move1, move2});
                } else if (moveValue == bestValue) {
                    bestMoves.push_back({move1, move2});
                }
            }
        }

        // Choose a random move among the best options
        return bestMoves[randomIndex(bestMoves.size())];
    }

private:
    static std::vector<std::vector<card>> generateCardPairs(const std::vector<card>& cards) {
        std::vector<std::vector<card>> pairs;
        for (size_t i = 0; i < cards.size(); ++i) {
            for (size_t j = i + 1; j < cards.size(); ++j) {
                pairs.push_back({cards[i], cards[j]});
            }
        }
        return pairs;
    }

    static int minimax(Board& board, 
                       const std::vector<card>& move1, 
                       const std::vector<card>& move2, 
                       int depth, bool isMaximizing, 
                       int alpha, int beta) {
        if (depth == 0) {
            return board.eval();
        }

        board.play(move1, move2); // Apply the moves

        std::vector<std::vector<card>> possibleMoves1 = generateCardPairs(board.getPlayer1Cards());
        std::vector<std::vector<card>> possibleMoves2 = generateCardPairs(board.getPlayer2Cards());

        if (isMaximizing) {
            int maxEval = std::numeric_limits<int>::min();
            for (const auto& nextMove1 : possibleMoves1) {
                for (const auto& nextMove2 : possibleMoves2) {
                    Board tempBoard = board;
                    int eval = minimax(tempBoard, nextMove1, nextMove2, depth - 1, false, alpha, beta);
                    maxEval = std::max(maxEval, eval);
                    alpha = std::max(alpha, eval);
                    if (beta <= alpha) break; // Alpha-beta pruning
                }
            }
            return maxEval;
        } else {
            int minEval = std::numeric_limits<int>::max();
            for (const auto& nextMove1 : possibleMoves1) {
                for (const auto& nextMove2 : possibleMoves2) {
                    Board tempBoard = board;
                    int eval = minimax(tempBoard, nextMove1, nextMove2, depth - 1, true, alpha, beta);
                    minEval = std::min(minEval, eval);
                    beta = std::min(beta, eval);
                    if (beta <= alpha) break; // Alpha-beta pruning
                }
            }
            return minEval;
        }
    }

    static size_t randomIndex(size_t size) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> dist(0, size - 1);
        return dist(gen);
    }
};
