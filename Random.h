#include <iostream>
#include <random>
#include <thread>

#include "Bots.h"
#include "Board.h"

class RandomBot: public Bot
{
	public:
	std::vector<BaseCard*> find_best_move(const Board& board, bool isPlayerRed)
	{
		auto moves = board.get_all_moves(isPlayerRed);
		if (moves.empty()) throw std::runtime_error("No moves available for the given board state.");
		
		static std::random_device rd;
		static std::mt19937 rng(rd());

		// static std::mt19937 rng(42); 
		std::uniform_int_distribution<int> distribution(0, moves.size() - 1);
	
		return moves[distribution(rng)];
	};
};

class GreedyBot: public Bot
{
	public:
	std::vector<BaseCard*> find_best_move(const Board& board, bool isPlayerRed)
	{
		auto moves = board.get_all_moves(isPlayerRed);
		if (moves.empty()) return {};
		auto opponent_moves = board.get_all_moves(!isPlayerRed);
		std::vector<std::vector<BaseCard*>> best_move;
		float best_score = -1000;
		for(auto& move : moves)
		{
			float score = 0;
			
			Board temp_board = board;
			temp_board.submit_turn_action(move, isPlayerRed);
			temp_board.submit_turn_action(opponent_moves[0], !isPlayerRed);
			score += temp_board.eval(isPlayerRed);

			// for(auto& opponent_move : opponent_moves)
			// {
			// 	Board temp_board = board;
			// 	temp_board.submit_turn_action(move, isPlayerRed);
			// 	temp_board.submit_turn_action(opponent_move, !isPlayerRed);
			// 	score += temp_board.eval(isPlayerRed);
			// }

			if(score > best_score)
			{
				best_score = score;
				best_move = {move};
			}
			else if(score == best_score)
			{
				best_move.push_back(move);
			}
		}
		static std::random_device rd;
		static std::mt19937 rng(rd());
		// static std::mt19937 rng(42); 
		std::uniform_int_distribution<int> distribution(0, best_move.size() - 1);
	
		return best_move[distribution(rng)];
	};
};

class RandomSumBot : public Bot 
{
	public:
		std::vector<BaseCard*> find_best_move(const Board& board, bool isPlayerRed) override {
			const int simulations_per_move = 100;
	
			auto all_moves = board.get_all_moves(isPlayerRed);
			std::vector<double> scores(all_moves.size(), 0.0);
	
			std::mt19937 rng(std::random_device{}());
	
			for (size_t i = 0; i < all_moves.size(); ++i) {
				int wins = 0, ties = 0;
	
				for (int sim = 0; sim < simulations_per_move; ++sim) {
					Board sim_board = board; // Deep copy of the board
					sim_board.submit_turn_action(all_moves[i], isPlayerRed);
	
					// Simulate random playout
					bool turn = !isPlayerRed;
					while (!sim_board.is_game_over()) {
						auto legal_moves = sim_board.get_all_moves(turn);
						if (legal_moves.empty()) break; // skip if no moves
						auto random_move = legal_moves[rng() % legal_moves.size()];
						sim_board.submit_turn_action(random_move, turn);
						turn = !turn;
					}
	
					int result = sim_board.game_winner(isPlayerRed);
					if (result == 1) wins++;
					else if (result == 0) ties++;
				}
	
				scores[i] = (wins + 0.5 * ties) / simulations_per_move;
			}
	
			// Pick the best move
			size_t best_index = 0;
			double best_score = -1.0;
			for (size_t i = 0; i < scores.size(); ++i) {
				if (scores[i] > best_score) {
					best_score = scores[i];
					best_index = i;
				}
			}
	
			return all_moves[best_index];
		}
};


class TimedRandomSumBot : public Bot {
	public:
		std::vector<BaseCard*> find_best_move(const Board& board, bool isPlayerRed) override {
			const int time_limit_ms = 3000;
	
			auto all_moves = board.get_all_moves(isPlayerRed);
			size_t num_moves = all_moves.size();
			if (num_moves == 0) return {}; // no legal moves
	
			std::vector<int> wins(num_moves, 0);
			std::vector<int> ties(num_moves, 0);
			std::vector<int> plays(num_moves, 0);
	
			std::mt19937 rng(std::random_device{}());
			auto start_time = std::chrono::high_resolution_clock::now();
			int loop_count = 0;
			while (true) {
				loop_count++;
				auto now = std::chrono::high_resolution_clock::now();
				if (std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count() >= time_limit_ms)
					break;
	
				size_t move_index = rng() % num_moves;
				Board sim_board = board;
				sim_board.submit_turn_action(all_moves[move_index], isPlayerRed);
	
				int result = simulate_random_playout(sim_board, !isPlayerRed, isPlayerRed, rng);
	
				plays[move_index]++;
				if (result == 1) wins[move_index]++;
				else if (result == 0) ties[move_index]++;
			}
			std::cout << "Loop count: " << loop_count << std::endl;
	
			// Pick best move
			double best_score = -1.0;
			size_t best_index = 0;
			for (size_t i = 0; i < num_moves; ++i) {
				if (plays[i] == 0) continue;
				double score = (wins[i] + 0.5 * ties[i]) / plays[i];
				if (score > best_score) {
					best_score = score;
					best_index = i;
				}
			}
	
			return all_moves[best_index];
		}
	
	private:
		int simulate_random_playout(Board board, bool turn, bool perspective, std::mt19937& rng) {
			while (!board.is_game_over()) {
				auto legal_moves = board.get_all_moves(turn);
				if (legal_moves.empty()) break;
				auto random_move = legal_moves[rng() % legal_moves.size()];
				board.submit_turn_action(random_move, turn);
				turn = !turn;
			}
	
			return board.game_winner(perspective); // result from bot's perspective
		}
};

class RandmSumParrallelBot : public Bot {
	public:
		std::vector<BaseCard*> find_best_move(const Board& board, bool isPlayerRed) override {
			const int time_limit_ms = 3000;
			const int num_threads = std::thread::hardware_concurrency();
			std::cout << "num_threads: " << num_threads << std::endl;
			auto all_moves = board.get_all_moves(isPlayerRed);
			size_t num_moves = all_moves.size();
			if (num_moves == 0) return {};
	
			std::vector<int> total_wins(num_moves, 0);
			std::vector<int> total_ties(num_moves, 0);
			std::vector<int> total_plays(num_moves, 0);
			std::mutex mutex;
	
			std::atomic<int> total_simulations(0); // NEW: for counting total playouts
	
			auto start_time = std::chrono::high_resolution_clock::now();
	
			auto worker = [&](int thread_id) {
				std::mt19937 rng(std::random_device{}());
				while (true) {
					auto now = std::chrono::high_resolution_clock::now();
					if (std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count() >= time_limit_ms)
						break;
	
					size_t move_index = rng() % num_moves;
					Board sim_board = board;
					sim_board.submit_turn_action(all_moves[move_index], isPlayerRed);
	
					int result = simulate_random_playout(sim_board, !isPlayerRed, isPlayerRed, rng);
	
					{
						std::lock_guard<std::mutex> lock(mutex);
						total_plays[move_index]++;
						if (result == 1) total_wins[move_index]++;
						else if (result == 0) total_ties[move_index]++;
					}
	
					total_simulations++; // increment atomic counter
				}
			};
	
			std::vector<std::thread> threads;
			for (int i = 0; i < num_threads; ++i) {
				threads.emplace_back(worker, i);
			}
			for (auto& t : threads) {
				t.join();
			}
	
			// Debug output
			std::cout << "[MCTS DEBUG] Total simulations: " << total_simulations.load() << std::endl;
	
			// Pick the best move
			double best_score = -1.0;
			size_t best_index = 0;
			for (size_t i = 0; i < num_moves; ++i) {
				if (total_plays[i] == 0) continue;
				double score = (total_wins[i] + 0.5 * total_ties[i]) / total_plays[i];
				if (score > best_score) {
					best_score = score;
					best_index = i;
				}
			}
	
			return all_moves[best_index];
		}
	
	private:
		int simulate_random_playout(Board board, bool turn, bool perspective, std::mt19937& rng) {
			while (!board.is_game_over()) {
				auto legal_moves = board.get_all_moves(turn);
				if (legal_moves.empty()) break;
				auto move = legal_moves[rng() % legal_moves.size()];
				board.submit_turn_action(move, turn);
				turn = !turn;
			}
			return board.game_winner(perspective);
		}
};
	