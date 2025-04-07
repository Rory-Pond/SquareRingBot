#pragma once
#include <vector>
#include <memory>
#include <random>
#include <cmath>
#include <unordered_map>

#include "constants.h"
#include "main.h"
#include "Random.h"

class MCTSNode {
public:
	Board state;
	std::vector<Action> move;
	int visits = 0;
	double wins = 0;
	std::vector<std::unique_ptr<MCTSNode>> children;
	MCTSNode* parent = nullptr;

	MCTSNode(const Board& state, std::vector<Action> move, MCTSNode* parent = nullptr)
		: state(state), move(std::move(move)), parent(parent) {}

};

class MCTSBot : public Bot {
public:
	std::vector<Action> find_best_move(const Board& board, bool isPlayerRed) override 
	{
		return run_mcts(board, isPlayerRed);
	}

private:
	static constexpr int SIMULATIONS = 50;
	// static constexpr double EXPLORATION_CONSTANT = std::sqrt(2);
	static constexpr double EXPLORATION_CONSTANT = 1.4142135623730951; // Approximation of sqrt(2)

	std::vector<Action> run_mcts(const Board& board, bool isPlayerRed) 
	{
		auto root = std::make_unique<MCTSNode>(board, std::vector<Action>{});

		for (int i = 0; i < SIMULATIONS; ++i) {
			MCTSNode* node = select_node(root.get());
			expand_node(node, isPlayerRed);
			int result = simulate(node->state, isPlayerRed);
			backpropagate(node, result, isPlayerRed);
		}
		
		return best_move(root.get());
	}

	MCTSNode* select_node(MCTSNode* node) {
		while (!node->children.empty()) {
			auto best_child = std::max_element(node->children.begin(), node->children.end(),
				[](const std::unique_ptr<MCTSNode>& a, const std::unique_ptr<MCTSNode>& b) {
					return ucb_score(a.get()) < ucb_score(b.get());
				});

			if (best_child == node->children.end()) {
				break; // Avoid undefined behavior
			}

			node = best_child->get();
		}
		return node;
	}

	void expand_node(MCTSNode* node, bool isPlayerRed) {
		if (node->state.is_game_over()) return;

		auto bot_move = node->state.get_all_moves(isPlayerRed);
		auto opp_move = node->state.get_all_moves(!isPlayerRed);
		static std::mt19937 rng(std::random_device{}());
		std::uniform_int_distribution<int> dist2(0, opp_move.size() - 1);
		
		for (const auto& test_move : bot_move) {
				Board new_state = node->state;
				new_state.play_action(test_move, opp_move[dist2(rng)]);
				node->children.push_back(std::make_unique<MCTSNode>(new_state, test_move, node));
		}
	}

	int simulate(Board state, bool isPlayerRed) {
		while (!state.is_game_over()) {
			auto pRed_moves = state.get_all_moves(true);
			auto pBlue_moves = state.get_all_moves(false);
			if (pRed_moves.empty() || pBlue_moves.empty()) break;

			static std::mt19937 rng(std::random_device{}());
			std::uniform_int_distribution<int> dist1(0, pRed_moves.size() - 1);
			std::uniform_int_distribution<int> dist2(0, pBlue_moves.size() - 1);

			state.play_action(pRed_moves[dist1(rng)], pBlue_moves[dist2(rng)]);
		}
		return state.game_winner();
	}

	void backpropagate(MCTSNode* node, int result, bool isPlayerRed) {
		while (node) {
			node->visits++;
			if ((result == 1 && isPlayerRed) || (result == -1 && !isPlayerRed)) {
				node->wins += 1.0;
			} else if (result == 0) {
				node->wins += 0.5;
			}
			node = node->parent;
		}
	}

	static double ucb_score(MCTSNode* node) {
		if (node->visits == 0) return std::numeric_limits<double>::max();
		return (node->wins / node->visits) + EXPLORATION_CONSTANT * std::sqrt(std::log(node->parent->visits) / node->visits);
	}

	std::vector<Action> best_move(MCTSNode* root) {
		auto best_child = std::max_element(root->children.begin(), root->children.end(),
			[](const std::unique_ptr<MCTSNode>& a, const std::unique_ptr<MCTSNode>& b) {
				return a->visits < b->visits;
			});
		return (*best_child)->move;
	}
};
