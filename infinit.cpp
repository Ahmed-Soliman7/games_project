#include <iostream>
#include <iomanip>
#include <cctype>
#include <deque>
#include <utility>
#include <algorithm>
#include "infinit.h"

using namespace std;

infinit_Board::infinit_Board() : Board(3, 3) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

bool infinit_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    if (x < 0 || x >= 3 || y < 0 || y >= 3 || board[x][y] != blank_symbol) {
        return false;
    }

    n_moves++;
    board[x][y] = toupper(mark);

    index.push_back({ x, y });

    if (n_moves >= 4 && (n_moves - 1) % 3 == 0) {
        if (!index.empty()) {
            board[index.front().first][index.front().second] = blank_symbol;
            index.pop_front();
        }
    }

    return true;
}

bool infinit_Board::is_win(Player<char>* player) {
    const char sym = player->get_symbol();

    // Check Rows
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == sym && board[i][1] == sym && board[i][2] == sym)
            return true;
    }

    // Check Columns
    for (int i = 0; i < 3; i++) {
        if (board[0][i] == sym && board[1][i] == sym && board[2][i] == sym)
            return true;
    }

    // Check Diagonals
    if ((board[0][0] == sym && board[1][1] == sym && board[2][2] == sym) ||
        board[0][2] == sym && board[1][1] == sym && board[2][0] == sym) {
        return true;
    }

    return false;
}

bool infinit_Board::is_draw(Player<char>* player) {
    // Check if board is full (no empty cells)
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == blank_symbol) {
                return false; // Board not full yet
            }
        }
    }
    // Board is full and no winner
    return !is_win(player);
}

bool infinit_Board::game_is_over(Player<char>* player) {

    return is_win(player) || is_draw(player);

}

infinit_AI_Player::infinit_AI_Player(string name, char symbol)
    : Player<char>(name, symbol, PlayerType::COMPUTER) {}

// Save current board state
infinit_AI_Player::BoardState infinit_AI_Player::save_state(infinit_Board* board) {
    BoardState state;

    // Copy the board vector
    state.board = board->get_board_matrix();
    state.move_history = board->get_move_history();
    state.n_moves = board->get_n_moves();

    return state;
}

// Restore board state (backtracking)
void infinit_AI_Player::restore_state(infinit_Board* board, const BoardState& state) {
    // Restore the board vector
    board->get_board_matrix() = state.board;
    board->get_move_history() = state.move_history;
    board->set_n_moves(state.n_moves);
}

// Check if there's an immediate threat (2 in a row)
bool infinit_AI_Player::has_immediate_threat(infinit_Board* board, char symbol) {
    vector<vector<char>>& b = board->get_board_matrix();

    // Check rows
    for (int i = 0; i < 3; i++) {
        int count = 0;
        for (int j = 0; j < 3; j++) {
            if (b[i][j] == symbol) count++;
        }
        if (count == 2) return true;
    }

    // Check columns
    for (int j = 0; j < 3; j++) {
        int count = 0;
        for (int i = 0; i < 3; i++) {
            if (b[i][j] == symbol) count++;
        }
        if (count == 2) return true;
    }

    // Check diagonals
    if ((b[0][0] == symbol && b[1][1] == symbol) ||
        (b[1][1] == symbol && b[2][2] == symbol) ||
        (b[0][0] == symbol && b[2][2] == symbol)) return true;

    if ((b[0][2] == symbol && b[1][1] == symbol) ||
        (b[1][1] == symbol && b[2][0] == symbol) ||
        (b[0][2] == symbol && b[2][0] == symbol)) return true;

    return false;
}

// Count winning opportunities for a symbol
int infinit_AI_Player::count_winning_opportunities(infinit_Board* board, char symbol) {
    vector<vector<char>>& b = board->get_board_matrix();
    int opportunities = 0;

    // Check rows
    for (int i = 0; i < 3; i++) {
        int count = 0, empty = 0;
        for (int j = 0; j < 3; j++) {
            if (b[i][j] == symbol) count++;
            else if (b[i][j] == '.') empty++;
        }
        if (count >= 1 && empty >= 1) opportunities++;
    }

    // Check columns
    for (int j = 0; j < 3; j++) {
        int count = 0, empty = 0;
        for (int i = 0; i < 3; i++) {
            if (b[i][j] == symbol) count++;
            else if (b[i][j] == '.') empty++;
        }
        if (count >= 1 && empty >= 1) opportunities++;
    }

    // Check diagonals
    int diag1_count = 0, diag1_empty = 0;
    int diag2_count = 0, diag2_empty = 0;

    for (int i = 0; i < 3; i++) {
        if (b[i][i] == symbol) diag1_count++;
        else if (b[i][i] == '.') diag1_empty++;

        if (b[i][2 - i] == symbol) diag2_count++;
        else if (b[i][2 - i] == '.') diag2_empty++;
    }

    if (diag1_count >= 1 && diag1_empty >= 1) opportunities++;
    if (diag2_count >= 1 && diag2_empty >= 1) opportunities++;

    return opportunities;
}

// Evaluate current board position
int infinit_AI_Player::evaluate_board(infinit_Board* board, char ai_symbol, char opponent_symbol) {
    Player<char> temp_ai("temp", ai_symbol, PlayerType::COMPUTER);
    Player<char> temp_opp("temp", opponent_symbol, PlayerType::COMPUTER);

    if (board->is_win(&temp_ai)) return 1000;
    if (board->is_win(&temp_opp)) return -1000;

    int score = 0;

    // Evaluate immediate threats
    if (has_immediate_threat(board, ai_symbol)) score += 50;
    if (has_immediate_threat(board, opponent_symbol)) score -= 60;

    // Evaluate winning opportunities
    score += count_winning_opportunities(board, ai_symbol) * 10;
    score -= count_winning_opportunities(board, opponent_symbol) * 10;

    // Evaluate center control
    vector<vector<char>>& b = board->get_board_matrix();
    if (b[1][1] == ai_symbol) score += 15;
    if (b[1][1] == opponent_symbol) score -= 15;

    // Evaluate corners
    int corners[4][2] = { {0,0}, {0,2}, {2,0}, {2,2} };
    for (auto& corner : corners) {
        if (b[corner[0]][corner[1]] == ai_symbol) score += 5;
        if (b[corner[0]][corner[1]] == opponent_symbol) score -= 5;
    }

    return score;
}

// Minimax algorithm with alpha-beta pruning and backtracking
int infinit_AI_Player::minimax(infinit_Board* board, int depth, bool is_maximizing,
    int alpha, int beta, char ai_symbol, char opponent_symbol) {

    Player<char> temp_ai("temp", ai_symbol, PlayerType::COMPUTER);
    Player<char> temp_opp("temp", opponent_symbol, PlayerType::COMPUTER);

    // Base cases
    if (board->is_win(&temp_ai)) return 1000 - depth;
    if (board->is_win(&temp_opp)) return -1000 + depth;
    if (depth >= MAX_DEPTH) return evaluate_board(board, ai_symbol, opponent_symbol);

    char current_symbol = is_maximizing ? ai_symbol : opponent_symbol;
    int best_score = is_maximizing ? INT_MIN : INT_MAX;

    // Try all possible moves with backtracking
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            vector<vector<char>>& b = board->get_board_matrix();
            if (b[i][j] == '.') {
                // Save state before move
                BoardState saved_state = save_state(board);

                // Try the move
                Move<char> move(i, j, current_symbol);
                board->update_board(&move);

                // Recursive call
                int score = minimax(board, depth + 1, !is_maximizing,
                    alpha, beta, ai_symbol, opponent_symbol);

                // Backtrack - restore original state
                restore_state(board, saved_state);

                // Update best score
                if (is_maximizing) {
                    best_score = max(best_score, score);
                    alpha = max(alpha, score);
                }
                else {
                    best_score = min(best_score, score);
                    beta = min(beta, score);
                }

                // Alpha-beta pruning
                if (beta <= alpha) {
                    return best_score;
                }
            }
        }
    }

    return best_score;
}

// Get best move for AI
Move<char>* infinit_AI_Player::get_move() {
    infinit_Board* board = dynamic_cast<infinit_Board*>(this->boardPtr);
    if (!board) return nullptr;

    int best_score = INT_MIN;
    int best_x = -1, best_y = -1;

    char ai_symbol = this->get_symbol();
    char opponent_symbol = (ai_symbol == 'X') ? 'O' : 'X';

    // Try all moves and find the best one with backtracking
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            vector<vector<char>>& b = board->get_board_matrix();
            if (b[i][j] == '.') {
                // Save state
                BoardState saved_state = save_state(board);

                // Try the move
                Move<char> move(i, j, ai_symbol);
                board->update_board(&move);

                // Evaluate the move
                int score = minimax(board, 0, false, INT_MIN, INT_MAX,
                    ai_symbol, opponent_symbol);

                // Backtrack
                restore_state(board, saved_state);

                // Update best move
                if (score > best_score) {
                    best_score = score;
                    best_x = i;
                    best_y = j;
                }
            }
        }
    }

    return new Move<char>(best_x, best_y, ai_symbol);
}

infinit_UI::infinit_UI() : UI<char>("Welcome to Infinite Tic Tac Toe", 3) {}

Player<char>* infinit_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    // Use AI player
    if (type == PlayerType::COMPUTER) {
        return new infinit_AI_Player(name, symbol);
    }

    return new Player<char>(name, symbol, type);
}

Move<char>* infinit_UI::get_move(Player<char>* player) {
    int x, y;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << player->get_name() << ", Please enter your move x and y (0 to 2): ";
        cin >> x >> y;
        return new Move<char>(x, y, player->get_symbol());
    }
    else {
        infinit_AI_Player* ai = dynamic_cast<infinit_AI_Player*>(player);
        if (ai) {
            return ai->get_move();
        }
        // Fallback
        x = rand() % 3;
        y = rand() % 3;
    }
}
