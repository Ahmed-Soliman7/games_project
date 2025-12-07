#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <climits>
#include "SUS.h"

using namespace std;

SUS_Board::SUS_Board() : Board(3, 3) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

bool SUS_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    if (!(x < 0 || x >= rows || y < 0 || y >= columns) && (board[x][y] == blank_symbol)) {
        char actual_mark = (mark == 'X') ? 'S' : 'U';
        board[x][y] = actual_mark;
        n_moves++;
        last_player_symbol = mark;
        check_sus_sequences();
        return true;
    }
    return false;
}

void SUS_Board::check_sus_sequences() {
    vector<vector<pair<int, int>>> all_sequences = {
        {{0,0}, {0,1}, {0,2}}, {{1,0}, {1,1}, {1,2}}, {{2,0}, {2,1}, {2,2}},
        {{0,0}, {1,0}, {2,0}}, {{0,1}, {1,1}, {2,1}}, {{0,2}, {1,2}, {2,2}},
        {{0,0}, {1,1}, {2,2}}, {{0,2}, {1,1}, {2,0}}
    };

    for (auto& sequence : all_sequences) {
        bool already_counted = false;
        for (auto& counted_seq : counted_sequences) {
            if (counted_seq == sequence) {
                already_counted = true;
                break;
            }
        }
        if (already_counted) continue;

        char c1 = board[sequence[0].first][sequence[0].second];
        char c2 = board[sequence[1].first][sequence[1].second];
        char c3 = board[sequence[2].first][sequence[2].second];

        if (c1 == 'S' && c2 == 'U' && c3 == 'S') {
            if (last_player_symbol == 'X') {
                sus_counter1++;
            }
            else if (last_player_symbol == 'O') {
                sus_counter2++;
            }
            counted_sequences.push_back(sequence);
        }
    }
}

bool SUS_Board::is_win(Player<char>* player) {
    if (n_moves == 9) {
        if (player->get_symbol() == 'X') {
            return sus_counter1 > sus_counter2;
        }
        else {
            return sus_counter2 > sus_counter1;
        }
    }
    return false;
}

bool SUS_Board::is_draw(Player<char>* player) {
    return (n_moves == 9 && sus_counter1 == sus_counter2);
}

bool SUS_Board::is_lose(Player<char>* player) {
    if (n_moves == 9) {
        if (player->get_symbol() == 'X') {
            return sus_counter1 < sus_counter2;
        }
        else {
            return sus_counter2 < sus_counter1;
        }
    }
    return false;
}

bool SUS_Board::game_is_over(Player<char>* player) {
    if (n_moves == 9) {
        cout << "Player X (S): " << sus_counter1 << " SUS patterns\n";
        cout << "Player O (U): " << sus_counter2 << " SUS patterns\n";

        if (sus_counter1 > sus_counter2)
            cout << "Player X WINS!" << endl;
        else if (sus_counter2 > sus_counter1)
            cout << "Player O WINS!" << endl;
        else
            cout << "It's a DRAW!" << endl;
        return true;
    }
    return false;
}

vector<pair<int, int>> SUS_Board::get_available_moves() {
    vector<pair<int, int>> moves;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (board[i][j] == blank_symbol) {
                moves.push_back({ i, j });
            }
        }
    }
    return moves;
}

bool SUS_Board::is_cell_empty(int x, int y) {
    return (x >= 0 && x < rows && y >= 0 && y < columns && board[x][y] == blank_symbol);
}

void SUS_Board::undo_move(int x, int y, int old_c1, int old_c2, size_t old_size) {
    board[x][y] = blank_symbol;
    n_moves--;
    sus_counter1 = old_c1;
    sus_counter2 = old_c2;
    counted_sequences.resize(old_size);
}

Smart_AI_Player::Smart_AI_Player(string name, char symbol, PlayerType type)
    : Player(name, symbol, type) {
}

int Smart_AI_Player::minimax(SUS_Board* board, bool maximizing, char ai_symbol) {
    if (board->get_n_moves() == 9) {
        int ai_score = (ai_symbol == 'X') ? board->get_sus_counter1() : board->get_sus_counter2();
        int opp_score = (ai_symbol == 'X') ? board->get_sus_counter2() : board->get_sus_counter1();

        if (ai_score > opp_score) return 100;
        if (ai_score < opp_score) return -100;
        return 0;
    }

    vector<pair<int, int>> moves = board->get_available_moves();

    if (maximizing) {
        int best = INT_MIN;
        for (auto& move : moves) {
            int old_c1 = board->get_sus_counter1();
            int old_c2 = board->get_sus_counter2();
            size_t old_size = board->get_n_moves();

            Move<char> m(move.first, move.second, ai_symbol);
            board->update_board(&m);

            int val = minimax(board, false, ai_symbol);

            board->undo_move(move.first, move.second, old_c1, old_c2, old_size);

            best = max(best, val);
        }
        return best;
    }
    else {
        int best = INT_MAX;
        char opp_symbol = (ai_symbol == 'X') ? 'O' : 'X';

        for (auto& move : moves) {
            int old_c1 = board->get_sus_counter1();
            int old_c2 = board->get_sus_counter2();
            size_t old_size = board->get_n_moves();

            Move<char> m(move.first, move.second, opp_symbol);
            board->update_board(&m);

            int val = minimax(board, true, ai_symbol);

            board->undo_move(move.first, move.second, old_c1, old_c2, old_size);

            best = min(best, val);
        }
        return best;
    }
}

pair<int, int> Smart_AI_Player::get_smart_move(SUS_Board* board) {
    vector<pair<int, int>> moves = board->get_available_moves();

    if (moves.empty()) return { -1, -1 };

    pair<int, int> best_move = moves[0];
    int best_score = INT_MIN;
    char ai_symbol = get_symbol();

    for (auto& move : moves) {
        int old_c1 = board->get_sus_counter1();
        int old_c2 = board->get_sus_counter2();
        size_t old_size = board->get_n_moves();

        Move<char> m(move.first, move.second, ai_symbol);
        board->update_board(&m);

        int score = minimax(board, false, ai_symbol);

        board->undo_move(move.first, move.second, old_c1, old_c2, old_size);

        if (score > best_score) {
            best_score = score;
            best_move = move;
        }
    }

    return best_move;
}

SUS_UI::SUS_UI() : UI<char>("Welcome to SUS Game!", 3) {}

Player<char>* SUS_UI::create_player(string& name, char symbol, PlayerType type) {
    if (type == PlayerType::HUMAN) {
        return new Player<char>(name, symbol, type);
    }
    else {
        return new Smart_AI_Player(name, symbol, type);
    }
}

Move<char>* SUS_UI::get_move(Player<char>* player) {
    int x, y;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << player->get_name() << " (" << player->get_symbol()
            << "), enter your move (row col): ";
        cin >> x >> y;
    }
    else {
        SUS_Board* board = dynamic_cast<SUS_Board*>(player->get_board_ptr());
        Smart_AI_Player* ai_player = dynamic_cast<Smart_AI_Player*>(player);

        if (ai_player && board) {
            pair<int, int> move = ai_player->get_smart_move(board);
            x = move.first;
            y = move.second;
            cout << player->get_name() << " (" << player->get_symbol()
                << ") chooses: " << x << " " << y << endl;
        }
        else {
            x = rand() % 3;
            y = rand() % 3;
        }
    }

    return new Move<char>(x, y, player->get_symbol());
}
