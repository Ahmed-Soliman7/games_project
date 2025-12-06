#ifndef infinit_CLASSES_H
#define infinit_CLASSES_H

#include "BoardGame_Classes.h"
#include <deque>
#include <utility>
#include <climits>
using namespace std;

class infinit_Board : public Board<char> {
private:
    char blank_symbol = '.';
    deque<pair<int, int>> index;

public:
    infinit_Board();
    bool update_board(Move<char>* move);
    bool is_lose(Player<char>* player) { return false; };
    bool is_win(Player<char>* player);
    bool is_draw(Player<char>* player);
    bool game_is_over(Player<char>* player);

    // Getters for AI access
    deque<pair<int, int>>& get_move_history() { return index; }
    int get_n_moves() const { return n_moves; }
    void set_n_moves(int moves) { n_moves = moves; }
    vector<vector<char>>& get_board_matrix() { return this->board; }
};

class infinit_AI_Player : public Player<char> {
private:
    static const int MAX_DEPTH = 6;

    // Structure to save board state for backtracking
    struct BoardState {
        vector<vector<char>> board;
        deque<pair<int, int>> move_history;
        int n_moves;
    };

    // Save current board state
    BoardState save_state(infinit_Board* board);

    // Restore board state (backtracking)
    void restore_state(infinit_Board* board, const BoardState& state);

    // Minimax algorithm with alpha-beta pruning and backtracking
    int minimax(infinit_Board* board, int depth, bool is_maximizing,
        int alpha, int beta, char ai_symbol, char opponent_symbol);

    // Evaluate current board position
    int evaluate_board(infinit_Board* board, char ai_symbol, char opponent_symbol);

    // Count winning opportunities for a symbol
    int count_winning_opportunities(infinit_Board* board, char symbol);

    // Check if there's a threat (2 in a row)
    bool has_immediate_threat(infinit_Board* board, char symbol);

public:
    infinit_AI_Player(string name, char symbol);
    Move<char>* get_move();
};

class infinit_UI : public UI<char> {
public:
    infinit_UI();
    ~infinit_UI() {};
    Player<char>* create_player(string& name, char symbol, PlayerType type);
    Move<char>* get_move(Player<char>* player);
};

#endif
