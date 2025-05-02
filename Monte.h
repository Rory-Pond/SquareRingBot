#include <random>

#include "Bots.h"

struct Node {
    Board state;
    bool isPlayerRed;
    std::vector<BaseCard*> move; // move taken from parent to get here
    Node* parent;
    std::vector<std::unique_ptr<Node>> children;

    int visits = 0;
    int wins = 0;
    bool fully_expanded = false;

    Node(Board state, bool isPlayerRed, std::vector<BaseCard*> move = {}, Node* parent = nullptr)
        : state(std::move(state)), isPlayerRed(isPlayerRed), move(std::move(move)), parent(parent) {}

    bool is_leaf() const {
        return children.empty();
    }

    bool is_terminal() const {
        return state.is_game_over();
    }

    double uct_score(int total_simulations, double exploration_constant = std::sqrt(2)) const {
        if (visits == 0) return std::numeric_limits<double>::infinity();
        double win_rate = static_cast<double>(wins) / visits;
        return win_rate + exploration_constant * std::sqrt(std::log(total_simulations) / visits);
    }
};

class MCTSBot : public Bot {
public:
    std::vector<BaseCard*> find_best_move(const Board& board, bool isPlayerRed) override {
        auto root = std::make_unique<Node>(board, isPlayerRed);

        std::mt19937 rng(std::random_device{}());
        const int time_limit_ms = 10000;
        auto start_time = std::chrono::high_resolution_clock::now();

        int total_simulations = 0;

        while (true) {
            auto now = std::chrono::high_resolution_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count() >= time_limit_ms)
                break;

            Node* node = select(root.get());
            Node* expanded = expand(node, rng);
            int result = simulate(expanded->state, expanded->isPlayerRed, isPlayerRed, rng);
            backpropagate(expanded, result);
            total_simulations++;
        }

        std::cout << "[UCT MCTS] Total simulations: " << total_simulations << std::endl;

        // Choose child with most visits
        Node* best = nullptr;
        int best_visits = -1;
        for (const auto& child : root->children) {
            if (child->visits > best_visits) {
                best_visits = child->visits;
                best = child.get();
            }
        }

        return best ? best->move : std::vector<BaseCard*>{};
    }

private:
    Node* select(Node* node) {
        while (!node->is_leaf() && !node->is_terminal()) {
            Node* best_child = nullptr;
            double best_uct = -1;
            for (const auto& child : node->children) {
                double uct = child->uct_score(node->visits);
                if (uct > best_uct) {
                    best_uct = uct;
                    best_child = child.get();
                }
            }
            node = best_child;
        }
        return node;
    }

    Node* expand(Node* node, std::mt19937& rng) {
        if (node->is_terminal()) return node;

        if (node->children.empty()) {
            auto moves = node->state.get_all_moves(node->isPlayerRed);
            for (const auto& move : moves) {
                Board new_state = node->state;
                new_state.submit_turn_action(move, node->isPlayerRed);
                node->children.push_back(std::make_unique<Node>(new_state, !node->isPlayerRed, move, node));
            }
            std::shuffle(node->children.begin(), node->children.end(), rng);
        }

        return node->children.front().get(); // could do better with unvisited first
    }

    int simulate(Board board, bool turn, bool perspective, std::mt19937& rng) {
        while (!board.is_game_over()) {
            auto legal = board.get_all_moves(turn);
            if (legal.empty()) break;
            board.submit_turn_action(legal[rng() % legal.size()], turn);
            turn = !turn;
        }
        return board.game_winner(perspective);
    }

    void backpropagate(Node* node, int result) {
        while (node) {
            node->visits++;
            if (result == 1) node->wins++;
            node = node->parent;
        }
    }
};