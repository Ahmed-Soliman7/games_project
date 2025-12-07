#ifndef fivexfive_CLASSES_H
#define fivexfive_CLASSES_H

#include "BoardGame_Classes.h"

using namespace std;

class fivexfive_Board : public Board<char> {
private:
    char blank_symbol = '.'; 
    int player1_score = 0;  
    int player2_score = 0;

    int count_three_in_a_row(char symbol);

    // New AI functions
    int evaluate_move(int x, int y, char symbol);  // Evaluate a specific move

public:
    fivexfive_Board();

    bool update_board(Move<char>* move) override;

    bool is_win(Player<char>* player) override;

    bool is_lose(Player<char>* player) override { return false; }

    bool is_draw(Player<char>* player) override;

    bool game_is_over(Player<char>* player) override;

    int get_score(char symbol);

    // New AI functions
    pair<int, int> find_best_move(char symbol);  // Find the best move
};


class fivexfive_UI : public UI<char> {
public:
    fivexfive_UI();

    ~fivexfive_UI() {};

    Player<char>* create_player(string& name, char symbol, PlayerType type) override;

    Move<char>* get_move(Player<char>* player) override;
};

#endif
