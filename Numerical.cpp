#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include "NumericalTTT.h"
using namespace std;


NumericalTTT_Board::NumericalTTT_Board(int rows, int cols)
    : Board<int>(rows, cols), n_moves(0) {

    for (int i = 0; i < 10; i++) used[i] = false;
}

Player<int>** NumericalTTT_UI::setup_players() {
    Player<int>** players = new Player<int>*[2];

    cout << "\n=== Player 1 (ODD numbers: 1, 3, 5, 7, 9) ===\n";
    string name1;
    cout << "Enter Player 1 name: ";
    cin >> name1;
    cout << "Choose Player 1 type:\n1. Human\n2. Computer\n> ";
    int choice1; cin >> choice1;
    PlayerType type1 = (choice1 == 2) ? PlayerType::COMPUTER : PlayerType::HUMAN;
    players[0] = create_player(name1, 0, type1);

    cout << "\n=== Player 2 (EVEN numbers: 2, 4, 6, 8) ===\n";
    string name2;
    if (type1 == PlayerType::COMPUTER) {
        cout << "Enter Player 2 name (or press Enter for 'Computer'): ";
        cin.ignore();
        getline(cin, name2);
        if (name2.empty()) name2 = "Computer";
    }
    else {
        cout << "Enter Player 2 name: ";
        cin >> name2;
    }
    cout << "Choose Player 2 type:\n1. Human\n2. Computer\n> ";
    int choice2; cin >> choice2;
    PlayerType type2 = (choice2 == 2) ? PlayerType::COMPUTER : PlayerType::HUMAN;
    players[1] = create_player(name2, 0, type2);

    cout << "\nGame starting...\n";
    cout << players[0]->get_name() << " (Odd)  vs  " << players[1]->get_name() << " (Even)\n\n";

    return players;
}
bool NumericalTTT_Board::update_board(Move<int>* move) {
    int x = move->get_x();
    int y = move->get_y();
    int num = move->get_symbol();

    bool odd_player = (n_moves % 2 == 0);
    bool error = false;
    string errors = "HAS ERROR! please fix the following\n";

    if ((odd_player && num % 2 == 0) || (!odd_player && num % 2 == 1)) {
        if (odd_player)
            errors += "Error: Player 1 can only play ODD numbers (1,3,5,7,9)\n";
        else
            errors += "Error: Player 2 can only play EVEN numbers (2,4,6,8)\n";
        error = true;
    }
    if (num < 1 || num > 9) {
        errors += "Invalid number : You must enter a number between 1 and 9.\n";
        error = true;
    }
    if (used[num]) {
        errors += "Error: Number " + to_string(num) + " has already been played!\n";
        errors += "Choose a number that hasn't been used yet.\n";
        error = true;
    }
    if (x < 0 || x >= 3 || y < 0 || y >= 3) {
        errors += "Invalid Position : Row and column must be between 0 and 2.\n";
        error = true;
    }
    if (board[x][y] != 0) {
        errors += "Invalid cell :The cell is already taken!\n";
        error = true;
    }

    if (error) {
        cout << errors << endl;
        return false;
    }

    board[x][y] = num;
    used[num] = true;
    n_moves++;
    return true;
}

bool NumericalTTT_Board::is_win(Player<int>* player) {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] && board[i][1] && board[i][2] &&
            board[i][0] + board[i][1] + board[i][2] == 15) return true;
        if (board[0][i] && board[1][i] && board[2][i] &&
            board[0][i] + board[1][i] + board[2][i] == 15) return true;
    }
    if (board[0][0] && board[1][1] && board[2][2] &&
        board[0][0] + board[1][1] + board[2][2] == 15) return true;
    if (board[0][2] && board[1][1] && board[2][0] &&
        board[0][2] + board[1][1] + board[2][0] == 15) return true;

    return false;
}

bool NumericalTTT_Board::is_draw(Player<int>* player) {
    return (n_moves == 9 && !is_win(player));
}

bool NumericalTTT_Board::game_is_over(Player<int>* player) {
    return is_win(player) || is_draw(player);
}

NumericalTTT_UI::NumericalTTT_UI()
    : UI<int>("Welcome to Numerical Tic-Tac-Toe!", 3) {}

Player<int>* NumericalTTT_UI::create_player(string& name, int, PlayerType type) {
    return new Player<int>(name, 0, type);
}

int NumericalTTT_UI::evaluate(vector<vector<int>>& board) {
    int score = 0;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == 0) continue;
            int sum = 0;
            int empty = 0;

            for (int k = 0; k < 3; k++) {
                if (board[i][k] != 0) sum += board[i][k];
                else empty++;
            }
            if (empty == 0 && sum == 15) score += 100;
            else if (empty == 1 && sum < 15) score += 10;

            sum = 0; empty = 0;
            for (int k = 0; k < 3; k++) {
                if (board[k][j] != 0) sum += board[k][j];
                else empty++;
            }
            if (empty == 0 && sum == 15) score += 100;
            else if (empty == 1 && sum < 15) score += 10;
        }
    }

    int sum = 0, empty = 0;
    for (int i = 0; i < 3; i++) {
        if (board[i][i] != 0) sum += board[i][i];
        else empty++;
    }
    if (empty == 0 && sum == 15) score += 100;
    else if (empty == 1 && sum < 15) score += 10;

    sum = 0; empty = 0;
    for (int i = 0; i < 3; i++) {
        if (board[i][2 - i] != 0) sum += board[i][2 - i];
        else empty++;
    }
    if (empty == 0 && sum == 15) score += 100;
    else if (empty == 1 && sum < 15) score += 10;

    return score;
}

int NumericalTTT_UI::minimax(vector<vector<int>> board, vector<bool> used,
    int moves, bool is_ai, int depth) {
    if (check_win(board)) return is_ai ? -10 : 10;
    if (moves == 9 || depth > 2) return evaluate(board);

    if (is_ai) {
        int best = -1000;
        bool odd_turn = (moves % 2 == 0);

        for (int num = 1; num <= 9; num++) {
            if (used[num] || (odd_turn && num % 2 == 0) || (!odd_turn && num % 2 == 1)) continue;

            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (board[i][j] == 0) {
                        board[i][j] = num;
                        used[num] = true;
                        best = max(best, minimax(board, used, moves + 1, false, depth + 1));
                        board[i][j] = 0;
                        used[num] = false;
                    }
                }
            }
        }
        return best;
    }
    else {
        int best = 1000;
        bool odd_turn = (moves % 2 == 0);

        for (int num = 1; num <= 9; num++) {
            if (used[num] || (odd_turn && num % 2 == 0) || (!odd_turn && num % 2 == 1)) continue;

            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (board[i][j] == 0) {
                        board[i][j] = num;
                        used[num] = true;
                        best = min(best, minimax(board, used, moves + 1, true, depth + 1));
                        board[i][j] = 0;
                        used[num] = false;
                    }
                }
            }
        }
        return best;
    }
}

bool NumericalTTT_UI::check_win(const vector<vector<int>>& board) {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] && board[i][1] && board[i][2] &&
            board[i][0] + board[i][1] + board[i][2] == 15) return true;
        if (board[0][i] && board[1][i] && board[2][i] &&
            board[0][i] + board[1][i] + board[2][i] == 15) return true;
    }
    if (board[0][0] && board[1][1] && board[2][2] &&
        board[0][0] + board[1][1] + board[2][2] == 15) return true;
    if (board[0][2] && board[1][1] && board[2][0] &&
        board[0][2] + board[1][1] + board[2][0] == 15) return true;
    return false;
}

Move<int>* NumericalTTT_UI::get_move(Player<int>* player) {
    NumericalTTT_Board* b = static_cast<NumericalTTT_Board*>(player->get_board_ptr());
    auto matrix = b->get_board_matrix();

    int x, y, num;
    int moves_count = 0;

    cout << "\n   0   1   2\n\n";
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (matrix[i][j] != 0)
                moves_count++;

    for (int i = 0; i < 3; i++) {
        cout << i << " ";
        for (int j = 0; j < 3; j++) {
            int val = matrix[i][j];
            if (val == 0) cout << " . ";
            else cout << " " << val << " ";
            if (j < 2) cout << "|";
        }
        cout << "\n";
        if (i < 2) cout << "  -----------\n";
    }
    cout << "\n";

    if (player->get_type() == PlayerType::HUMAN) {
        cout << player->get_name()
            << ", Please enter your number and position (num x y): ";
        cin >> num >> x >> y;
    }
    else {
        vector<int> available;
        vector<bool> used(10, false);
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (matrix[i][j] != 0)
                    used[matrix[i][j]] = true;

        bool odd_turn = (moves_count % 2 == 0);

        for (int n = 1; n <= 9; n++) {
            if (!used[n]) {
                if ((odd_turn && n % 2 == 1) || (!odd_turn && n % 2 == 0))
                    available.push_back(n);
            }
        }

        int best_score = -100;
        num = available[0];
        x = y = 0;

        for (int cand_num : available) {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (matrix[i][j] == 0) {
                        auto test_board = matrix;
                        auto test_used = used;
                        test_board[i][j] = cand_num;
                        test_used[cand_num] = true;

                        int score = minimax(test_board, test_used, moves_count + 1, false, 0);

                        if (score > best_score) {
                            best_score = score;
                            num = cand_num;
                            x = i;
                            y = j;
                        }
                    }
                }
            }
        }

        cout << "Computer plays " << num << " at (" << x << ", " << y << ")\n";
    }

    return new Move<int>(x, y, num);
}
