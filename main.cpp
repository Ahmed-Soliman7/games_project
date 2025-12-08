#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cstdlib>

#include "BoardGame_Classes.h"

#include "misere.h"
#include "fourinfour.h"
#include "NumericalTTT.h"
#include "obstacles.h"
#include "infinit.h"
#include "fivexfive.h"
#include "SUS.h"
#include "pyramid.h"
#include "ultimate.h"
#include "Diamond.h"
#include "memory.h"
#include "word.h"
#include "Four-in-a-row.h"

using namespace std;

void games() {
    cout << "1. Misere Tic Tac Toe" << endl;
    cout << "2. 4 x 4 Tic-Tac-Toe" << endl;
    cout << "3. 3 x 3 Numerical Tic-Tac-Toe" << endl;
    cout << "4. Obstacles Tic-Tac-Toe" << endl;
    cout << "5. Infinit Tic-Tac-Toe" << endl;
    cout << "6. 5 x 5 Tic-Tac-Toe" << endl;
    cout << "7. SUS Tic-Tac-Toe" << endl;
    cout << "8. Pyramid Tic-Tac-Toe" << endl;
    cout << "9. Ultimate Tic Tac Toe" << endl;
    cout << "10. Diamond Tic-Tac-Toe" << endl;
    cout << "11. Memory Tic-Tac-Toe" << endl;
    cout << "12. Word Tic-Tac-Toe" << endl;
    cout << "13. Four-in-a-row Tic-Tac-Toe" << endl;
    cout << "0. Exit" << endl;
}

void game_torun() {
    int choice;
    cin >> choice;
    if (choice == 0) {
        exit(0);
    }
    else if (choice == 1) {
        srand(static_cast<unsigned int>(time(0)));

        UI<char>* game_ui = new misere_UI();

        Board<char>* misere_board = new misere_Board();

        Player<char>** players = game_ui->setup_players();

        GameManager<char> misere_game(misere_board, players, game_ui);

        misere_game.run();

        delete misere_board;

        for (int i = 0; i < 2; ++i) {
            delete players[i];
        }
        delete[] players;
    }
    else if (choice == 2) {
        srand(static_cast<unsigned int>(time(0)));

        UI<char>* game_ui = new fourinfour_UI();

        Board<char>* fourinfour_board = new fourinfour_Board();

        Player<char>** players = game_ui->setup_players();

        GameManager<char> fourinfour_game(fourinfour_board, players, game_ui);

        fourinfour_game.run();

        delete fourinfour_board;

        for (int i = 0; i < 2; ++i) {
            delete players[i];
        }
        delete[] players;

    }
    else if (choice == 3) {
        srand(static_cast<unsigned int>(time(nullptr)));

        NumericalTTT_UI* game_ui = new NumericalTTT_UI();
        NumericalTTT_Board* num_board = new NumericalTTT_Board(3, 3);

        Player<int>** players = game_ui->setup_players();


        players[0]->set_board_ptr(num_board);
        players[1]->set_board_ptr(num_board);

        GameManager<int> game(num_board, players, game_ui);
        game.run();


        delete num_board;
        delete players[0];
        delete players[1];
        delete[] players;
        delete game_ui;
    }
    else if (choice == 4) {
        srand(static_cast<unsigned int>(time(0)));

        UI<char>* game_ui = new obstacles_UI();

        Board<char>* obstacles_board = new obstacles_Board();

        Player<char>** players = game_ui->setup_players();

        GameManager<char> obstacles_game(obstacles_board, players, game_ui);

        obstacles_game.run();

        delete obstacles_board;

        for (int i = 0; i < 2; ++i) {
            delete players[i];
        }
        delete[] players;
    }
    else if (choice == 5) {
        srand(static_cast<unsigned int>(time(0)));

        UI<char>* game_ui = new infinit_UI();

        Board<char>* infinit_board = new infinit_Board();

        Player<char>** players = game_ui->setup_players();

        GameManager<char> fourinfour_game(infinit_board, players, game_ui);

        fourinfour_game.run();

        delete infinit_board;

        for (int i = 0; i < 2; ++i) {
            delete players[i];
        }
        delete[] players;
    }
    else if (choice == 6) {
        srand(static_cast<unsigned int>(time(0)));

        UI<char>* game_ui = new fivexfive_UI();
        Board<char>* fivexfive_board = new fivexfive_Board();
        Player<char>** players = game_ui->setup_players();

        GameManager<char> game(fivexfive_board, players, game_ui);
        game.run();

        delete fivexfive_board;
        for (int i = 0; i < 2; ++i) {
            delete players[i];
        }
        delete[] players;
        delete game_ui;
    }
    else if (choice == 7) {
        srand(static_cast<unsigned int>(time(0)));

        UI<char>* game_ui = new SUS_UI();
        Board<char>* sus_board = new SUS_Board();
        Player<char>** players = game_ui->setup_players();

        GameManager<char> sus_game(sus_board, players, game_ui);
        sus_game.run();

        // Cleanup
        delete sus_board;
        delete game_ui;
        for (int i = 0; i < 2; ++i) {
            delete players[i];
        }
        delete[] players;
    }
    else if (choice == 8) {
        srand(static_cast<unsigned int>(time(0)));

        UI<char>* game_ui = new pyramid_UI();
        Board<char>* pyramid_board = new pyramid_Board();
        Player<char>** players = game_ui->setup_players();

        GameManager<char> pyramid_game(pyramid_board, players, game_ui);
        pyramid_game.run();

        // Cleanup
        delete pyramid_board;
        delete game_ui;
        for (int i = 0; i < 2; ++i) {
            delete players[i];
        }
        delete[] players;
    }
    else if (choice == 9) {
        srand(static_cast<unsigned int>(time(0)));

        UI<char>* game_ui = new ultimate_UI();
        Board<char>* ultimate_board = new ultimate_Board();
        Player<char>** players = game_ui->setup_players();

        GameManager<char> ultimate_game(ultimate_board, players, game_ui);
        ultimate_game.run();

        delete ultimate_board;
        for (int i = 0; i < 2; ++i) {
            delete players[i];
        }
        delete[] players;
        delete game_ui;
    }
    else if (choice == 10) {

        srand(static_cast<unsigned int>(time(0)));

        UI<char>* game_ui = new Diamond_UI();

        Board<char>* diamond_board = new Diamond_Board();

        Player<char>** players = game_ui->setup_players();

        GameManager<char> diamond_game(diamond_board, players, game_ui);

        diamond_game.run();

        delete diamond_board;
        delete game_ui;

        for (int i = 0; i < 2; ++i) {
            delete players[i];
        }
        delete[] players;
    }
    else if (choice == 11) {
        srand(static_cast<unsigned int>(time(0)));

        memory_UI* game_ui = new memory_UI();

        Board<char>* memory_board = new memory_Board();

        Player<char>** players = game_ui->setup_players();

        GameManager<char> memory_game(memory_board, players, game_ui);

        memory_game.run();

        game_ui->display_revealed_board(memory_board->get_board_matrix());

        delete memory_board;
        delete game_ui;

        for (int i = 0; i < 2; ++i) {
            delete players[i];
        }
        delete[] players;
    }
    else if (choice == 12) {
        srand(static_cast<unsigned int>(time(0)));

        UI<char>* game_ui = new Word_UI();

        Board<char>* word_board = new Word_Board();

        Player<char>** players = game_ui->setup_players();

        GameManager<char> word_game(word_board, players, game_ui);

        word_game.run();

        delete word_board;
        delete game_ui;

        for (int i = 0; i < 2; ++i) {
            delete players[i];
        }
    }
    else if (choice == 13) {

        srand(static_cast<unsigned int>(time(0)));

        UI<char>* game_ui = new Four_in_a_row_UI();

        Board<char>* Four_in_a_row_board = new Four_in_a_row_Board();

        Player<char>** players = game_ui->setup_players();

        GameManager<char> Four_in_a_row_game(Four_in_a_row_board, players, game_ui);

        Four_in_a_row_game.run();

        delete Four_in_a_row_board;
        delete game_ui;

        for (int i = 0; i < 2; ++i) {
            delete players[i];
        }
        delete[] players;
    }
}

int main() {
    cout << "Hello in EL Masateel games!" << endl;
    cout << "Please choose the game to run : " << endl;
    while (true) {
        games();
        game_torun();
    }
    return 0;

}
