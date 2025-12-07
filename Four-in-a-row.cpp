#include <iostream>
#include <iomanip>
#include <cctype>
#include <climits>
#include "Four-in-a-row.h"

using namespace std;

Four_in_a_row_Board::Four_in_a_row_Board() : Board(6, 7) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

int Four_in_a_row_Board::get_lowest_empty_row(int col) {
    for (int row = rows - 1; row >= 0; --row) {
        if (board[row][col] == blank_symbol) {
            return row;
        }
    }
    return -1;
}

bool Four_in_a_row_Board::update_board(Move<char>* move) {
    int col = move->get_y();
    char mark = move->get_symbol();

    if (col < 0 || col >= columns) {
        return false;
    }

    if (mark == 0) {
        for (int r = 0; r < rows; ++r) {
            if (board[r][col] != blank_symbol) {
                board[r][col] = blank_symbol;
                n_moves--;
                return true;
            }
        }
        return false;
    }

    int row = get_lowest_empty_row(col);
    if (row < 0) {
        return false;
    }

    board[row][col] = toupper(mark);
    n_moves++;
    return true;
}

bool Four_in_a_row_Board::is_win(Player<char>* player) {
    const char sym = player->get_symbol();

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < columns - 3; ++col) {
            if (board[row][col] == sym &&
                board[row][col + 1] == sym &&
                board[row][col + 2] == sym &&
                board[row][col + 3] == sym) {
                return true;
            }
        }
    }

    for (int row = 0; row < rows - 3; ++row) {
        for (int col = 0; col < columns; ++col) {
            if (board[row][col] == sym &&
                board[row + 1][col] == sym &&
                board[row + 2][col] == sym &&
                board[row + 3][col] == sym) {
                return true;
            }
        }
    }

    for (int row = 0; row < rows - 3; ++row) {
        for (int col = 0; col < columns - 3; ++col) {
            if (board[row][col] == sym &&
                board[row + 1][col + 1] == sym &&
                board[row + 2][col + 2] == sym &&
                board[row + 3][col + 3] == sym) {
                return true;
            }
        }
    }

    for (int row = 0; row < rows - 3; ++row) {
        for (int col = 3; col < columns; ++col) {
            if (board[row][col] == sym &&
                board[row + 1][col - 1] == sym &&
                board[row + 2][col - 2] == sym &&
                board[row + 3][col - 3] == sym) {
                return true;
            }
        }
    }

    return false;
}

bool Four_in_a_row_Board::is_lose(Player<char>* player) {
    return false;
}

bool Four_in_a_row_Board::is_draw(Player<char>* player) {
    return (n_moves == 42 && !is_win(player));
}

bool Four_in_a_row_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

vector<int> Four_in_a_row_Board::get_available_columns() {
    vector<int> cols;
    for (int c = 0; c < columns; c++) {
        if (get_lowest_empty_row(c) >= 0) {
            cols.push_back(c);
        }
    }
    return cols;
}

bool Four_in_a_row_Board::is_column_full(int col) {
    return get_lowest_empty_row(col) < 0;
}

Smart_Four_AI::Smart_Four_AI(string name, char symbol, PlayerType type)
    : Player(name, symbol, type) {
}

int Smart_Four_AI::count_windows(Four_in_a_row_Board* board, char symbol) {
    int score = 0;
    int rows = board->get_rows();
    int cols = board->get_columns();

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols - 3; c++) {
            int count = 0, empty = 0;
            for (int i = 0; i < 4; i++) {
                char cell = board->get_cell(r, c + i);
                if (cell == symbol) count++;
                else if (cell == '.') empty++;
            }
            if (count == 4) score += 100;
            else if (count == 3 && empty == 1) score += 5;
            else if (count == 2 && empty == 2) score += 2;
        }
    }

    for (int r = 0; r < rows - 3; r++) {
        for (int c = 0; c < cols; c++) {
            int count = 0, empty = 0;
            for (int i = 0; i < 4; i++) {
                char cell = board->get_cell(r + i, c);
                if (cell == symbol) count++;
                else if (cell == '.') empty++;
            }
            if (count == 4) score += 100;
            else if (count == 3 && empty == 1) score += 5;
            else if (count == 2 && empty == 2) score += 2;
        }
    }

    for (int r = 0; r < rows - 3; r++) {
        for (int c = 0; c < cols - 3; c++) {
            int count = 0, empty = 0;
            for (int i = 0; i < 4; i++) {
                char cell = board->get_cell(r + i, c + i);
                if (cell == symbol) count++;
                else if (cell == '.') empty++;
            }
            if (count == 4) score += 100;
            else if (count == 3 && empty == 1) score += 5;
            else if (count == 2 && empty == 2) score += 2;
        }
    }

    for (int r = 0; r < rows - 3; r++) {
        for (int c = 3; c < cols; c++) {
            int count = 0, empty = 0;
            for (int i = 0; i < 4; i++) {
                char cell = board->get_cell(r + i, c - i);
                if (cell == symbol) count++;
                else if (cell == '.') empty++;
            }
            if (count == 4) score += 100;
            else if (count == 3 && empty == 1) score += 5;
            else if (count == 2 && empty == 2) score += 2;
        }
    }

    return score;
}

int Smart_Four_AI::evaluate_position(Four_in_a_row_Board* board, char symbol) {
    char opp = (symbol == 'X') ? 'O' : 'X';
    return count_windows(board, symbol) - count_windows(board, opp);
}

int Smart_Four_AI::minimax(Four_in_a_row_Board* board, int depth, bool maximizing,
    int alpha, int beta, char ai_symbol) {
    Player<char> temp_player("temp", ai_symbol, PlayerType::COMPUTER);
    temp_player.set_board_ptr(board);

    char opp_symbol = (ai_symbol == 'X') ? 'O' : 'X';
    Player<char> opp_player("opp", opp_symbol, PlayerType::COMPUTER);
    opp_player.set_board_ptr(board);

    if (depth == 0 || board->get_n_moves() == 42) {
        return evaluate_position(board, ai_symbol);
    }

    if (board->is_win(&temp_player)) {
        return 10000;
    }
    if (board->is_win(&opp_player)) {
        return -10000;
    }

    vector<int> cols = board->get_available_columns();

    if (maximizing) {
        int max_val = INT_MIN;
        for (int col : cols) {
            Move<char> m(0, col, ai_symbol);
            board->update_board(&m);

            int val = minimax(board, depth - 1, false, alpha, beta, ai_symbol);

            Move<char> undo(0, col, 0);
            board->update_board(&undo);

            max_val = max(max_val, val);
            alpha = max(alpha, val);
            if (beta <= alpha) break;
        }
        return max_val;
    }
    else {
        int min_val = INT_MAX;
        for (int col : cols) {
            Move<char> m(0, col, opp_symbol);
            board->update_board(&m);

            int val = minimax(board, depth - 1, true, alpha, beta, ai_symbol);

            Move<char> undo(0, col, 0);
            board->update_board(&undo);

            min_val = min(min_val, val);
            beta = min(beta, val);
            if (beta <= alpha) break;
        }
        return min_val;
    }
}

int Smart_Four_AI::get_smart_move(Four_in_a_row_Board* board) {
    vector<int> cols = board->get_available_columns();
    if (cols.empty()) return -1;

    int best_col = cols[0];
    int best_score = INT_MIN;
    char ai_symbol = get_symbol();

    for (int col : cols) {
        Move<char> m(0, col, ai_symbol);
        board->update_board(&m);

        int score = minimax(board, 5, false, INT_MIN, INT_MAX, ai_symbol);

        Move<char> undo(0, col, 0);
        board->update_board(&undo);

        if (score > best_score) {
            best_score = score;
            best_col = col;
        }
    }

    return best_col;
}

Four_in_a_row_UI::Four_in_a_row_UI() : UI<char>("Welcome to Four-in-a-row", 8) {}

Player<char>* Four_in_a_row_UI::create_player(string& name, char symbol, PlayerType type) {
    if (type == PlayerType::HUMAN) {
        return new Player<char>(name, symbol, type);
    }
    else {
        return new Smart_Four_AI(name, symbol, type);
    }
}

Move<char>* Four_in_a_row_UI::get_move(Player<char>* player) {
    int col;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << player->get_name() << ", please enter a column (0 to 6): ";
        cin >> col;
        return new Move<char>(0, col, player->get_symbol());
    }
    else {
        Four_in_a_row_Board* board = dynamic_cast<Four_in_a_row_Board*>(player->get_board_ptr());
        Smart_Four_AI* ai = dynamic_cast<Smart_Four_AI*>(player);

        if (ai && board) {
            col = ai->get_smart_move(board);
            cout << player->get_name() << " chooses column: " << col << endl;
        }
        else {
            col = rand() % 7;
        }
        return new Move<char>(0, col, player->get_symbol());
    }
}
