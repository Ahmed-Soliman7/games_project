#ifndef obstacles_CLASSES_H
#define obstacles_CLASSES_H

#include "BoardGame_Classes.h"

using namespace std;

class obstacles_Board : public Board<char> {
private:
    char blank_symbol = '.'; ///< Character used to represent an empty cell on the board.

public:
    obstacles_Board();

    bool update_board(Move<char>* move) override;

    bool is_win(Player<char>* player) override;

    bool is_lose(Player<char>* player) override { return false; }

    bool is_draw(Player<char>* player) override;

    bool game_is_over(Player<char>* player) override;

};


class obstacles_UI : public UI<char> {
private:
    char blank_symbol = '.'; ///< Character used to represent an empty cell on the board.
public:
    obstacles_UI();

    ~obstacles_UI() {};

    Player<char>* create_player(string& name, char symbol, PlayerType type) override;

    Move<char>* get_move(Player<char>* player) override;
};

#endif
