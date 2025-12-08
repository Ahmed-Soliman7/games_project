#ifndef WORD_CLASSES_H
#define WORD_CLASSES_H

#include "BoardGame_Classes.h"
#include <set>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

class Word_Board : public Board<char> {
private:
    char blank_symbol = '.';
    set<string> dictionary; 
    
    void load_dictionary();

public:
    Word_Board();
    
    bool update_board(Move<char>* move);
    
    bool check_word(int row, int col, int dx, int dy);
    
    char get_cell(int x, int y) { return board[x][y]; }
    
    void set_cell(int x, int y, char value) { board[x][y] = value; }
    
    bool is_win(Player<char>* player);
    
    bool is_lose(Player<char>*) { return false; };
    
    bool is_draw(Player<char>* player);
    
    bool game_is_over(Player<char>* player);
};


class Word_UI : public UI<char> {
public:
    Word_UI();
    
    ~Word_UI() {};
    
    Player<char>* create_player(string& name, char symbol, PlayerType type);
    
    virtual Move<char>* get_move(Player<char>* player);
};

#endif
