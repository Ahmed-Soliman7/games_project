#pragma once
#ifndef SUS_CLASSES_H
#define SUS_CLASSES_H

#include "BoardGame_Classes.h"
#include <vector>
#include <utility>
using namespace std;

class SUS_Board : public Board<char> {
private:
    char blank_symbol = '.';
    int sus_counter1 = 0;
    int sus_counter2 = 0;
    char last_player_symbol;
    vector<vector<pair<int, int>>> counted_sequences;

public:
    SUS_Board();
    bool update_board(Move<char>* move) override;
    bool is_lose(Player<char>* player) override;
    bool is_win(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
    void check_sus_sequences();
    vector<pair<int, int>> get_available_moves();
    bool is_cell_empty(int x, int y);
    int get_sus_counter1() const { return sus_counter1; }
    int get_sus_counter2() const { return sus_counter2; }
    void undo_move(int x, int y, int old_c1, int old_c2, size_t old_size);
};

class Smart_AI_Player : public Player<char> {
private:
    int minimax(SUS_Board* board, bool maximizing, char ai_symbol);

public:
    Smart_AI_Player(string name, char symbol, PlayerType type);
    ~Smart_AI_Player() {}
    pair<int, int> get_smart_move(SUS_Board* board);
};

class SUS_UI : public UI<char> {
public:
    SUS_UI();
    ~SUS_UI() {};
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* player) override;
};

#endif
