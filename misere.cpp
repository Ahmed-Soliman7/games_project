#include <iostream>
#include <iomanip>
#include <cctype>
#include <functional>  
#include <vector>      
#include <algorithm>
#include "misere.h"

using namespace std;

misere_Board::misere_Board() : Board(3, 3) {
    // Initialize all cells with blank_symbol
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

bool misere_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();
    //Check if coordinates are out of bounds
    if (x < 0 || x >= rows || y < 0 || y >= columns) {
        cout << "invalid coordinates position must be between 0 to 2 \n";
        return false;
    }
    // Check if cell is already taken 
    if (mark != 0 && board[x][y] != blank_symbol) {
         cout << "Is already taken  please choose an empty cell" << endl;
        return false;
    }
    // Validate move and apply if valid
    if (board[x][y] == blank_symbol || mark == 0) {

        if (mark == 0) {
            n_moves--;
            board[x][y] = blank_symbol;
        }
        else {
            n_moves++;
            board[x][y] = toupper(mark);
        }
        return true;
    }
    return false;
}

bool misere_Board::is_lose(Player<char>* player) {
    const char sym = player->get_symbol();

    auto all_equal = [&](char a, char b, char c) {
        return a == b && b == c && a != blank_symbol;
        };

    // Check rows and columns
    for (int i = 0; i < rows; ++i) {
        if ((all_equal(board[i][0], board[i][1], board[i][2]) && board[i][0] == sym) ||
            (all_equal(board[0][i], board[1][i], board[2][i]) && board[0][i] == sym))
            return true;
    }

    // Check diagonals
    if ((all_equal(board[0][0], board[1][1], board[2][2]) && board[1][1] == sym) ||
        (all_equal(board[0][2], board[1][1], board[2][0]) && board[1][1] == sym))
        return true;

    return false;
}

bool misere_Board::is_draw(Player<char>* player) {
    return (n_moves == 9 && !is_lose(player));
}

bool misere_Board::game_is_over(Player<char>* player) {
    return is_lose(player) || is_draw(player);
}

misere_UI::misere_UI() : UI<char>("Weclome to Mesere Tic Tac Toe", 3) {}

Player<char>* misere_UI::create_player(string& name, char symbol, PlayerType type) {
    // Create player based on type
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

Move<char>* misere_UI::get_move(Player<char>* player) {
    int x, y;
    
    if (player->get_type() == PlayerType::HUMAN) {
        cout << player -> get_name() << " ,Please enter your move x and y (0 to 2): ";
        cin >> x >> y;
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
       Board<char>* board_ptr = player->get_board_ptr();
        char symbol = player->get_symbol();
        vector<vector<char>> board_matrix = board_ptr->get_board_matrix();
        char opp_symbol = (symbol == 'X') ? 'O' : 'X';

        // Check if a player has three in a row 
        auto check_lose = [](vector<vector<char>>& b, char sym) -> bool {
            auto all_equal = [&](char a, char b, char c) {
                return a == b && b == c && a != '.';
                };

            // Check rows and columns
            for (int i = 0; i < 3; ++i) {
                if ((all_equal(b[i][0], b[i][1], b[i][2]) && b[i][0] == sym) ||
                    (all_equal(b[0][i], b[1][i], b[2][i]) && b[0][i] == sym))
                    return true;
            }

            // Check diagonals
            if ((all_equal(b[0][0], b[1][1], b[2][2]) && b[1][1] == sym) ||
                (all_equal(b[0][2], b[1][1], b[2][0]) && b[1][1] == sym))
                return true;

            return false;
            };

        // Count moves on board
        auto count_moves = [](vector<vector<char>>& b) -> int {
            int count = 0;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (b[i][j] != '.') count++;
                }
            }
            return count;
            };

        // Evaluation function for Misere 
        auto evaluate = [&](vector<vector<char>>& b) -> int {
            if (check_lose(b, symbol)) return -1000;  // lose
            if (check_lose(b, opp_symbol)) return 1000;  // win

            int score = 0;

            
            //having 2 in a row is dangerous!
            for (int i = 0; i < 3; i++) {
                // Check rows
                int my_count = 0, opp_count = 0, empty_count = 0;
                for (int j = 0; j < 3; j++) {
                    if (b[i][j] == symbol) my_count++;
                    else if (b[i][j] == opp_symbol) opp_count++;
                    else empty_count++;
                }
                if (my_count == 2 && empty_count == 1) score -= 50;  
                if (opp_count == 2 && empty_count == 1) score += 50;  

                // Check columns
                my_count = opp_count = empty_count = 0;
                for (int j = 0; j < 3; j++) {
                    if (b[j][i] == symbol) my_count++;
                    else if (b[j][i] == opp_symbol) opp_count++;
                    else empty_count++;
                }
                if (my_count == 2 && empty_count == 1) score -= 50;
                if (opp_count == 2 && empty_count == 1) score += 50;
            }

            // Check diagonals
            int my_d1 = 0, opp_d1 = 0, empty_d1 = 0;
            int my_d2 = 0, opp_d2 = 0, empty_d2 = 0;

            for (int i = 0; i < 3; i++) {
                if (b[i][i] == symbol) my_d1++;
                else if (b[i][i] == opp_symbol) opp_d1++;
                else empty_d1++;

                if (b[i][2 - i] == symbol) my_d2++;
                else if (b[i][2 - i] == opp_symbol) opp_d2++;
                else empty_d2++;
            }

            if (my_d1 == 2 && empty_d1 == 1) score -= 50;
            if (opp_d1 == 2 && empty_d1 == 1) score += 50;
            if (my_d2 == 2 && empty_d2 == 1) score -= 50;
            if (opp_d2 == 2 && empty_d2 == 1) score += 50;

            return score;
            };

     
       function<int(vector<vector<char>>&, int, bool, int, int)> minimax =
            [&](vector<vector<char>>& b, int depth, bool is_maximizing, int alpha, int beta) -> int {

            // Terminal states
            if (check_lose(b, symbol)) return -1000 + depth;  //  bad
            if (check_lose(b, opp_symbol)) return 1000 - depth;  // good
            if (count_moves(b) == 9) return 0;  // Draw
            if (depth >= 6) return evaluate(b);  // Depth limit

            char current_player = is_maximizing ? symbol : opp_symbol;
            int best_score = is_maximizing ? INT_MIN : INT_MAX;

            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (b[i][j] == '.') {
                        // Make move
                        b[i][j] = current_player;

                        int score = minimax(b, depth + 1, !is_maximizing, alpha, beta);

                        // Undo move
                        b[i][j] = '.';

                        if (is_maximizing) {
                            best_score = max(best_score, score);
                            alpha = max(alpha, score);
                        }
                        else {
                            best_score = min(best_score, score);
                            beta = min(beta, score);
                        }

                        // Alpha-beta pruning
                        if (beta <= alpha) break;
                    }
                }
                if (beta <= alpha) break;
            }

            return best_score;
            };

        // Find best move
        int best_score = INT_MIN;
        int best_x = -1, best_y = -1;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board_matrix[i][j] == '.') {
                    // Try this move
                    board_matrix[i][j] = symbol;

                    int score = minimax(board_matrix, 0, false, INT_MIN, INT_MAX);

                    // Undo move
                    board_matrix[i][j] = '.';

                    if (score > best_score) {
                        best_score = score;
                        best_x = i;
                        best_y = j;
                    }
                }
            }
        }

        // If we found a move
        if (best_x != -1) {
            x = best_x;
            y = best_y;
            cout << "Computer plays: (" << x << ", " << y << ") [Score: " << best_score << "]\n";
        }
        else {
            // random
            do {
                x = rand() % 3;
                y = rand() % 3;
            } while (board_matrix[x][y] != '.');
        }
    }

    return new Move<char>(x, y, player->get_symbol());
}
