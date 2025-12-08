#include <iostream>
#include <iomanip>
#include <cctype>
#include <algorithm>
#include <vector>
#include "word.h"

using namespace std;

Word_Board::Word_Board() : Board(3, 3) {
    
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
    
    load_dictionary();
}

void Word_Board::load_dictionary() {
    ifstream file("dic.txt");
    string word;
    
    while (file >> word) {
        
        transform(word.begin(), word.end(), word.begin(), ::toupper);
        if (word.length() == 3) {
            dictionary.insert(word);
        }
    }
    file.close();
}

bool Word_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    
    if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
        (board[x][y] == blank_symbol || mark == 0)) {

        if (mark == 0) { // undo move
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

bool Word_Board::check_word(int row, int col, int dx, int dy) {
    string word = "";
    
    
    for (int i = 0; i < 3; i++) {
        int newRow = row + i * dx;
        int newCol = col + i * dy;
        
        if (newRow < 0 || newRow >= rows || newCol < 0 || newCol >= columns)
            return false;
            
        if (board[newRow][newCol] == blank_symbol)
            return false;
            
        word += board[newRow][newCol];
    }
    
    
    return dictionary.count(word) > 0;
}

bool Word_Board::is_win(Player<char>* player) {
    // Check all possible starting positions for 3-letter words
    
    // Check rows (horizontal)
    for (int i = 0; i < rows; i++) {
        if (check_word(i, 0, 0, 1)) // left to right
            return true;
    }
    
    // Check columns (vertical)
    for (int j = 0; j < columns; j++) {
        if (check_word(0, j, 1, 0)) // top to bottom
            return true;
    }
    
    // Check diagonals
    if (check_word(0, 0, 1, 1)) // main diagonal (top-left to bottom-right)
        return true;
    
    if (check_word(0, 2, 1, -1)) // anti-diagonal (top-right to bottom-left)
        return true;
    
    return false;
}

bool Word_Board::is_draw(Player<char>* player) {
    return (n_moves == 9 && !is_win(player));
}

bool Word_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

Word_UI::Word_UI() : UI<char>("Welcome to Word Tic Tac Toe", 3) {}

Player<char>* Word_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

Move<char>* Word_UI::get_move(Player<char>* player) {
    int x, y;
    char letter;
    
    if (player->get_type() == PlayerType::HUMAN) {
        cout << player->get_name() << ", Please enter position (x y) and letter: ";
        cin >> x >> y >> letter;
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        Word_Board* board = dynamic_cast<Word_Board*>(player->get_board_ptr());
        bool found_move = false;
        
        // Try all empty cells with all letters
        for (int i = 0; i < 3 && !found_move; i++) {
            for (int j = 0; j < 3 && !found_move; j++) {
                if (board->get_cell(i, j) == '.') {
                    // Try every letter
                    for (char c = 'A'; c <= 'Z' && !found_move; c++) {

                        // Save original state
                        board->set_cell(i, j, c);
                        
                        // If this move wins, take it!
                        if (board->is_win(player)) {
                            x = i;
                            y = j;
                            letter = c;
                            found_move = true;
                        }
                        
                        // Restore the cell
                        board->set_cell(i, j, '.');
                    }
                }
            }
        }
        
        // If no winning move found, try to block or complete a word
        if (!found_move) {
            for (int i = 0; i < 3 && !found_move; i++) {
                for (int j = 0; j < 3 && !found_move; j++) {
                    if (board->get_cell(i, j) == '.') {
                        for (char c = 'A'; c <= 'Z' && !found_move; c++) {

                            board->set_cell(i, j, c);
                            
                            // If this move completes a word in any direction
                            if (board->check_word(i, 0, 0, 1) || 
                                board->check_word(0, j, 1, 0) ||
                                board->check_word(0, 0, 1, 1) ||
                                board->check_word(0, 2, 1, -1)) {

                                x = i;
                                y = j;
                                letter = c;
                                found_move = true;
                            }
                            
                            board->set_cell(i, j, '.');
                        }
                    }
                }
            }
        }
        
        // If still no move, pick random empty spot
        if (!found_move) {
            vector<pair<int, int>> empty_cells;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (board->get_cell(i, j) == '.') {
                        empty_cells.push_back({i, j});
                    }
                }
            }
            
            if (!empty_cells.empty()) {
                int random_idx = rand() % empty_cells.size();
                x = empty_cells[random_idx].first;
                y = empty_cells[random_idx].second;
                letter = 'A' + (rand() % 26);
            }
        }
    }
    
    return new Move<char>(x, y, toupper(letter));
}
