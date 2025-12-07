#ifndef ultimate_CLASSES_H
#define ultimate_CLASSES_H

#include <vector>
#include <utility>
#include "BoardGame_Classes.h"
using namespace std;

// External variables used for meta-board
extern vector<int> index;
extern vector<char> win;

// Ultimate Tic Tac Toe Board
class ultimate_Board : public Board<char> {
private:
    char blank_symbol = '.'; // Blank cell symbol
    void check_subboard_wins();
    char check_subboard_winner(int r_start, int r_end, int c_start, int c_end);
    void mark_closed_boards();

public:
    ultimate_Board();
    bool update_board(Move<char>* move);
    bool is_lose(Player<char>* player) { return false; }
    bool is_win(Player<char>*);
    bool is_draw(Player<char>* player);
    bool game_is_over(Player<char>* player);
    void reset_board();

    // Getters for AI access
    char get_cell(int x, int y) const { return board[x][y]; }
    char get_blank_symbol() const { return blank_symbol; }
};

// Ultimate Tic Tac Toe UI
class ultimate_UI : public UI<char> {
public:
    ultimate_UI();
    ~ultimate_UI() {};

    Player<char>* create_player(string& name, char symbol, PlayerType type);
    virtual Move<char>* get_move(Player<char>* player) override;
    void display_meta_board(const vector<char>& win_array);
    virtual void display_board_matrix(const vector<vector<char>>& matrix) const override;
};

#endif
