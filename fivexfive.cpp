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

    // Check horizontal three-in-a-rows
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == symbol &&
                board[i][j + 1] == symbol &&
                board[i][j + 2] == symbol) {
                count++;
            }
        }
    }

    // Check vertical three-in-a-rows
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            if (board[i][j] == symbol &&
                board[i + 1][j] == symbol &&
                board[i + 2][j] == symbol) {
                count++;
            }
        }
    }

    // Check diagonal (top-left to bottom-right)
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == symbol &&
                board[i + 1][j + 1] == symbol &&
                board[i + 2][j + 2] == symbol) {
                count++;
            }
        }
    }

    // Check diagonal (top-right to bottom-left)
    for (int i = 0; i < 3; i++) {
        for (int j = 2; j < 5; j++) {
            if (board[i][j] == symbol &&
                board[i + 1][j - 1] == symbol &&
                board[i + 2][j - 2] == symbol) {
                count++;
            }
        }
    }

    return count;
}

int fivexfive_Board::get_score(char symbol) {
    return count_three_in_a_row(symbol);
}

bool fivexfive_Board::is_win(Player<char>* player) {
    // Game doesn't end until 24 moves (only 1 square left empty)
    // So we check for winner based on score when game ends
    return false;
}

bool fivexfive_Board::is_draw(Player<char>* player) {
    // Game ends when 24 moves are made (only 1 empty square remains)
    if (n_moves >= 24) {
        // Count scores for both players
        player1_score = count_three_in_a_row('X');
        player2_score = count_three_in_a_row('O');

        // Display final scores
        cout << "\n=== GAME OVER ===" << endl;
        cout << "Player X score: " << player1_score << endl;
        cout << "Player O score: " << player2_score << endl;

        return (player1_score == player2_score);
    }
    return false;
}

bool fivexfive_Board::game_is_over(Player<char>* player) {
    if (n_moves >= 24) {
        // Count scores for both players
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

    // Display current move number
    fivexfive_Board* board_ptr = static_cast<fivexfive_Board*>(player->get_board_ptr());

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\n" << player->get_name() << " (" << player->get_symbol() << "), enter your move (row column) [0-4]: ";
        cin >> x >> y;
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        // Random computer player
        do {
            x = rand() % 5;
            y = rand() % 5;
        } while (board_ptr->get_board_matrix()[x][y] != '.');

        cout << "Computer (" << player->get_symbol() << ") plays at (" << x << ", " << y << ")" << endl;
    }

    return new Move<char>(x, y, player->get_symbol());
}

