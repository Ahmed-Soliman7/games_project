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
#include "SUS.h"

using namespace std;

void games() {
    cout << "1. SUS" << endl;
    cout << "2. Misere Tic Tac Toe" << endl;
    cout << "3. 4 x 4 Tic-Tac-Toe" << endl;
    cout << "4. 3 x 3 Numerical Tic-Tac-Toe" << endl;
    cout << "5. Obstacles Tic-Tac-Toe" << endl;
    cout << "6. Infinit Tic-Tac-Toe" << endl;
    cout << "0. Exit" << endl;
}

void game_torun() {
    char choice;
    cin >> choice;
    if (choice == '0') {
        exit(0);
    }
    else if (choice == '1') {
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
    else if (choice == '2') {
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
    else if (choice == '3') {
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
    else if (choice == '4') {
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
    else if (choice == '5') {
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
    else if (choice == '6') {
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
