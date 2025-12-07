#include <iostream>
#include <iomanip>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include "fivexfive.h"

using namespace std;

fivexfive_Board::fivexfive_Board() : Board(5, 5) {
    // Initialize all cells with blank_symbol
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

bool fivexfive_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    // Validate move
    if (x < 0 || x >= rows || y < 0 || y >= columns || board[x][y] != blank_symbol) {
        return false;
    }

    // Place the mark
    n_moves++;
    board[x][y] = toupper(mark);

    return true;
}

int fivexfive_Board::count_three_in_a_row(char symbol) {
    int count = 0;
    symbol = toupper(symbol);

    // Horizontal
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == symbol &&
                board[i][j+1] == symbol &&
                board[i][j+2] == symbol) {
                count++;
            }
        }
    }

    // Vertical
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            if (board[i][j] == symbol &&
                board[i+1][j] == symbol &&
                board[i+2][j] == symbol) {
                count++;
            }
        }
    }

    // Diagonal (top-left to bottom-right)
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == symbol &&
                board[i+1][j+1] == symbol &&
                board[i+2][j+2] == symbol) {
                count++;
            }
        }
    }

    // Diagonal (top-right to bottom-left)
    for (int i = 0; i < 3; i++) {
        for (int j = 2; j < 5; j++) {
            if (board[i][j] == symbol &&
                board[i+1][j-1] == symbol &&
                board[i+2][j-2] == symbol) {
                count++;
            }
        }
    }

    return count;
}

// Evaluate move for the AI
int fivexfive_Board::evaluate_move(int x, int y, char symbol) {
    if (board[x][y] != blank_symbol) return -1000;
    
    int score = 0;
    char opponent = (symbol == 'X') ? 'O' : 'X';
    
    char original = board[x][y];
    
    // Simulate move
    board[x][y] = symbol;
    int my_threes = count_three_in_a_row(symbol);
    board[x][y] = original;
    int old_threes = count_three_in_a_row(symbol);

    score += (my_threes - old_threes) * 100;

    // Block opponent
    board[x][y] = opponent;
    int opp_threes = count_three_in_a_row(opponent);
    board[x][y] = original;
    int old_opp_threes = count_three_in_a_row(opponent);

    score += (old_opp_threes - opp_threes) * 80;

    // Future potential
    board[x][y] = symbol;

    int potential = 0;

    // Horizontal potential
    for (int j = max(0, y-2); j <= min(2, y); j++) {
        int spaces = 0, my_marks = 0;
        for (int k = 0; k < 3; k++) {
            if (board[x][j+k] == symbol) my_marks++;
            else if (board[x][j+k] == blank_symbol) spaces++;
        }
        if (my_marks >= 1 && spaces >= 1) potential += my_marks * 5;
    }

    // Vertical potential
    for (int i = max(0, x-2); i <= min(2, x); i++) {
        int spaces = 0, my_marks = 0;
        for (int k = 0; k < 3; k++) {
            if (board[i+k][y] == symbol) my_marks++;
            else if (board[i+k][y] == blank_symbol) spaces++;
        }
        if (my_marks >= 1 && spaces >= 1) potential += my_marks * 5;
    }

    // Main diagonal potential
    for (int offset = -2; offset <= 0; offset++) {
        int startX = x + offset;
        int startY = y + offset;
        if (startX >= 0 && startY >= 0 && startX + 2 < 5 && startY + 2 < 5) {
            int spaces = 0, my_marks = 0;
            for (int k = 0; k < 3; k++) {
                if (board[startX+k][startY+k] == symbol) my_marks++;
                else if (board[startX+k][startY+k] == blank_symbol) spaces++;
            }
            if (my_marks >= 1 && spaces >= 1) potential += my_marks * 5;
        }
    }

    // Anti-diagonal potential
    for (int offset = -2; offset <= 0; offset++) {
        int startX = x + offset;
        int startY = y - offset;
        if (startX >= 0 && startY < 5 && startX + 2 < 5 && startY - 2 >= 0) {
            int spaces = 0, my_marks = 0;
            for (int k = 0; k < 3; k++) {
                if (board[startX+k][startY-k] == symbol) my_marks++;
                else if (board[startX+k][startY-k] == blank_symbol) spaces++;
            }
            if (my_marks >= 1 && spaces >= 1) potential += my_marks * 5;
        }
    }

    score += potential;

    int distFromCenter = abs(x - 2) + abs(y - 2);
    score += (4 - distFromCenter) * 3;

    score += rand() % 10;

    board[x][y] = original;
    return score;
}

pair<int, int> fivexfive_Board::find_best_move(char symbol) {
    int bestScore = -10000;
    int bestX = -1, bestY = -1;
    
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (board[i][j] == blank_symbol) {
                int score = evaluate_move(i, j, symbol);
                
                if (score > bestScore) {
                    bestScore = score;
                    bestX = i;
                    bestY = j;
                }
            }
        }
    }
    
    if (bestX == -1) {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (board[i][j] == blank_symbol) {
                    return {i, j};
                }
            }
        }
    }
    
    return {bestX, bestY};
}

int fivexfive_Board::get_score(char symbol) {
    return count_three_in_a_row(symbol);
}

bool fivexfive_Board::is_win(Player<char>* player) {
    return false;
}

bool fivexfive_Board::is_draw(Player<char>* player) {
    if (n_moves >= 24) {
        player1_score = count_three_in_a_row('X');
        player2_score = count_three_in_a_row('O');
        
        cout << "\n=== GAME OVER ===" << endl;
        cout << "Player X score: " << player1_score << endl;
        cout << "Player O score: " << player2_score << endl;
        
        return (player1_score == player2_score);
    }
    return false;
}

bool fivexfive_Board::game_is_over(Player<char>* player) {
    if (n_moves >= 24) {
        player1_score = count_three_in_a_row('X');
        player2_score = count_three_in_a_row('O');
        
        return true;
    }
    return false;
}

fivexfive_UI::fivexfive_UI() : UI<char>("Welcome to 5 x 5 Tic Tac Toe", 3) {
    cout << "Goal: Get the most three-in-a-row sequences!" << endl;
    cout << "Game ends after 24 moves (when only 1 square remains empty)" << endl;
}

Player<char>* fivexfive_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

Move<char>* fivexfive_UI::get_move(Player<char>* player) {
    int x, y;
    
    fivexfive_Board* board_ptr = static_cast<fivexfive_Board*>(player->get_board_ptr());
    
    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\n" << player->get_name() << " (" << player->get_symbol() << "), enter your move (row column) [0-4]: ";
        cin >> x >> y;
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        auto best_move = board_ptr->find_best_move(player->get_symbol());
        x = best_move.first;
        y = best_move.second;
        
        cout << "Computer (" << player->get_symbol() << ") plays at (" << x << ", " << y << ")" << endl;
    }

    return new Move<char>(x, y, player->get_symbol());
}
