#include <iostream>
#include <iomanip>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <utility>
#include <algorithm>
#include <functional>
#include "Diamond.h"

using namespace std;

Diamond_Board::Diamond_Board() : Board(7, 7) {
    // Initialize all cells
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            board[i][j] = ' ';
        }
    }

    board[0][3] = blank_symbol;
    for (int i = 2; i < 5; i++)
        board[1][i] = blank_symbol;
    for (int i = 1; i < 6; i++)
        board[2][i] = blank_symbol;
    for (int i = 0; i < 7; i++)
        board[3][i] = blank_symbol;
    for (int i = 1; i < 6; i++)
        board[4][i] = blank_symbol;
    for (int i = 2; i < 5; i++)
        board[5][i] = blank_symbol;
    board[6][3] = blank_symbol;
}

bool Diamond_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    if (x < 0 || x >= 7 || y < 0 || y >= 7 || board[x][y] != blank_symbol)
        return false;
    board[x][y] = toupper(move->get_symbol());
    n_moves++;
    return true;
}

bool Diamond_Board::check_line(int x, int y, int dir, int length, char sym) {
    const int dx[4] = { 1, 0, 1,  1 };
    const int dy[4] = { 0, 1, 1, -1 };

    for (int k = 0; k < length; ++k) {
        int xx = x + dx[dir] * k;
        int yy = y + dy[dir] * k;
        if (xx < 0 || xx >= 7 || yy < 0 || yy >= 7 || board[xx][yy] != sym)
            return false;
    }
    return true;
}

bool Diamond_Board::is_win(Player<char>* player) {
    const char sym = toupper(player->get_symbol());

    struct Line {
        vector<pair<int, int>> cells;
        int direction;
    };

    vector<Line> lines_of_3;
    vector<Line> lines_of_4;

    const int dx[4] = { 1, 0, 1,  1 };
    const int dy[4] = { 0, 1, 1, -1 };

    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 7; ++j) {
            if (board[i][j] != sym) continue;

            for (int dir = 0; dir < 4; ++dir) {
                // Check for line of 3
                if (check_line(i, j, dir, 3, sym)) {
                    Line line;
                    line.direction = dir;
                    for (int k = 0; k < 3; ++k) {
                        line.cells.push_back({ i + dx[dir] * k, j + dy[dir] * k });
                    }
                    lines_of_3.push_back(line);
                }
                // Check for line of 4
                if (check_line(i, j, dir, 4, sym)) {
                    Line line;
                    line.direction = dir;
                    for (int k = 0; k < 4; ++k) {
                        line.cells.push_back({ i + dx[dir] * k, j + dy[dir] * k });
                    }
                    lines_of_4.push_back(line);
                }
            }
        }
    }

    for (auto& line3 : lines_of_3) {
        for (auto& line4 : lines_of_4) {
            // Check different direction
            if (line3.direction == line4.direction)
                continue;

            // Count common points
            int common_points = 0;
            for (auto& p3 : line3.cells) {
                for (auto& p4 : line4.cells) {
                    if (p3 == p4) {
                        common_points++;
                    }
                }
            }

            // Win if lines in different directions
            // Can share one point
            if (common_points <= 1) {
                return true;
            }
        }
    }

    return false;
}

bool Diamond_Board::is_draw(Player<char>* player) {
    return n_moves == 25 && !is_win(player);
}

bool Diamond_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

Diamond_UI::Diamond_UI() : UI<char>("Welcome to Diamond Tic Tac Toe", 3) {}

Player<char>* Diamond_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";
    return new Player<char>(name, symbol, type);
}

Move<char>* Diamond_UI::get_move(Player<char>* player) {
    static const pair<int, int> cells[25] = {
         {0,3},
         {1,2},{1,3},{1,4},
         {2,1},{2,2},{2,3},{2,4},{2,5},
         {3,0},{3,1},{3,2},{3,3},{3,4},{3,5},{3,6},
         {4,1},{4,2},{4,3},{4,4},{4,5},
         {5,2},{5,3},{5,4},
         {6,3}
    };

    int num;
    Board<char>* b = player->get_board_ptr();

    if (player->get_type() == PlayerType::HUMAN) {
        cout << player->get_name() << " (" << player->get_symbol() << ")  cell (1-25): ";
        cin >> num;
        while (num < 1 || num > 25 ||
            b->get_cell(cells[num - 1].first, cells[num - 1].second) != blank_symbol) {
            cout << "Invalid try again: \n";
            cin >> num;
        }
    }
    else {

        vector<vector<char>> board_matrix = b->get_board_matrix();
        char symbol = player->get_symbol();
        char opp_symbol = (symbol == 'X') ? 'O' : 'X';

        // Check if symbol has a line of length
        auto check_line_on_board = [](vector<vector<char>>& brd, int x, int y, int dir, int length, char sym) -> bool {
            const int dx[4] = { 1, 0, 1,  1 };
            const int dy[4] = { 0, 1, 1, -1 };
            for (int k = 0; k < length; ++k) {
                int xx = x + dx[dir] * k;
                int yy = y + dy[dir] * k;
                if (xx < 0 || xx >= 7 || yy < 0 || yy >= 7 || brd[xx][yy] != sym)
                    return false;
            }
            return true;
            };

        //Check win condition on board
        auto check_win_on_board = [&](vector<vector<char>>& brd, char sym) -> bool {
            struct Line {
                vector<pair<int, int>> cells;
                int direction;
            };
            vector<Line> lines_of_3, lines_of_4;
            const int dx[4] = { 1, 0, 1,  1 };
            const int dy[4] = { 0, 1, 1, -1 };

            for (int i = 0; i < 7; ++i) {
                for (int j = 0; j < 7; ++j) {
                    if (brd[i][j] != sym) continue;
                    for (int dir = 0; dir < 4; ++dir) {
                        if (check_line_on_board(brd, i, j, dir, 3, sym)) {
                            Line line;
                            line.direction = dir;
                            for (int k = 0; k < 3; ++k)
                                line.cells.push_back({ i + dx[dir] * k, j + dy[dir] * k });
                            lines_of_3.push_back(line);
                        }
                        if (check_line_on_board(brd, i, j, dir, 4, sym)) {
                            Line line;
                            line.direction = dir;
                            for (int k = 0; k < 4; ++k)
                                line.cells.push_back({ i + dx[dir] * k, j + dy[dir] * k });
                            lines_of_4.push_back(line);
                        }
                    }
                }
            }

            for (auto& line3 : lines_of_3) {
                for (auto& line4 : lines_of_4) {
                    if (line3.direction == line4.direction) continue;
                    int common = 0;
                    for (auto& p3 : line3.cells)
                        for (auto& p4 : line4.cells)
                            if (p3 == p4) common++;
                    if (common <= 1) return true;
                }
            }
            return false;
            };

        //Evaluate board state
        auto evaluate = [&](vector<vector<char>>& brd) -> int {
            if (check_win_on_board(brd, symbol)) return 10000;       // AI wins
            if (check_win_on_board(brd, opp_symbol)) return -10000;  // Human wins

            int score = 0;
            const int dx[4] = { 1, 0, 1, 1 };
            const int dy[4] = { 0, 1, 1, -1 };

            // Count lines of different lengths
            for (int i = 0; i < 7; i++) {
                for (int j = 0; j < 7; j++) {
                    for (int dir = 0; dir < 4; dir++) {
                        // Line of 4 
                        if (check_line_on_board(brd, i, j, dir, 4, symbol)) score += 500;
                        if (check_line_on_board(brd, i, j, dir, 4, opp_symbol)) score -= 600;

                        // Line of 3 
                        if (check_line_on_board(brd, i, j, dir, 3, symbol)) score += 100;
                        if (check_line_on_board(brd, i, j, dir, 3, opp_symbol)) score -= 150;

                        // Line of 2 
                        if (check_line_on_board(brd, i, j, dir, 2, symbol)) score += 10;
                        if (check_line_on_board(brd, i, j, dir, 2, opp_symbol)) score -= 15;
                    }
                }
            }

            // Center control bonus
            if (brd[3][3] == symbol) score += 20;

            return score;
            };

        // Minimax with alpha-beta pruning
        function<int(vector<vector<char>>&, int, int, int, bool)> minimax =
            [&](vector<vector<char>>& brd, int depth, int alpha, int beta, bool is_max) -> int {

            if (check_win_on_board(brd, symbol)) return 10000 - depth;
            if (check_win_on_board(brd, opp_symbol)) return -10000 + depth;
            if (depth >= 3) return evaluate(brd); // Depth limit

            int best_score = is_max ? -99999 : 99999;

            for (int i = 0; i < 25; i++) {
                int x = cells[i].first;
                int y = cells[i].second;

                if (brd[x][y] == blank_symbol) {
                    // Make move
                    brd[x][y] = is_max ? symbol : opp_symbol;

                    // Recurse
                    int score = minimax(brd, depth + 1, alpha, beta, !is_max);

                    // Undo move (Backtracking)
                    brd[x][y] = blank_symbol;

                    // Update best score and alpha-beta
                    if (is_max) {
                        best_score = max(best_score, score);
                        alpha = max(alpha, score);
                    }
                    else {
                        best_score = min(best_score, score);
                        beta = min(beta, score);
                    }

                    // Alpha-Beta pruning
                    if (beta <= alpha) break;
                }
            }

            return best_score;
            };

        // Find best move
        int best_score = -99999;
        int best_move = -1;

        for (int i = 0; i < 25; i++) {
            int x = cells[i].first;
            int y = cells[i].second;

            if (board_matrix[x][y] == blank_symbol) {
                // Try move
                board_matrix[x][y] = symbol;

                // Evaluate using minimax
                int score = minimax(board_matrix, 0, -99999, 99999, false);

                // Undo move
                board_matrix[x][y] = blank_symbol;

                // Update best
                if (score > best_score) {
                    best_score = score;
                    best_move = i + 1;
                }
            }
        }

        // Fallback to random if no good move found
        if (best_move == -1) {
            vector<int> available;
            for (int i = 0; i < 25; i++) {
                int x = cells[i].first;
                int y = cells[i].second;
                if (b->get_cell(x, y) == blank_symbol) {
                    available.push_back(i + 1);
                }
            }
            if (!available.empty())
                best_move = available[rand() % available.size()];
        }

        num = best_move;
        cout << "Computer plays " << num << " " << symbol << endl;
    }

    int x = cells[num - 1].first;
    int y = cells[num - 1].second;
    return new Move<char>(x, y, player->get_symbol());
}

void Diamond_UI::display_board_matrix(const vector<vector<char>>& m) const {
    char c;

    // Row 0 – cell 1
    cout << "         ";
    cout << ' ' << m[0][3] << " \n";

    // Row 1 – cells 2 3 4
    cout << "      ";
    cout << ' ' << m[1][2] << ' ';
    cout << ' ' << m[1][3] << ' ';
    cout << ' ' << m[1][4] << "\n";

    // Row 2 – cells 5 6 7 8 9
    cout << "   ";
    cout << ' ' << m[2][1] << ' ';
    cout << ' ' << m[2][2] << ' ';
    cout << ' ' << m[2][3] << ' ';
    cout << ' ' << m[2][4] << ' ';
    cout << ' ' << m[2][5] << "\n";

    // Row 3 – cells 10 11 12 13 14 15 16
    cout << ' ' << m[3][0] << ' ';
    cout << ' ' << m[3][1] << ' ';
    cout << ' ' << m[3][2] << ' ';
    cout << ' ' << m[3][3] << ' ';
    cout << ' ' << m[3][4] << ' ';
    cout << ' ' << m[3][5] << ' ';
    cout << ' ' << m[3][6] << "\n";

    // Row 4 – cells 17 18 19 20 21 
    cout << "   ";
    cout << ' ' << m[4][1] << ' ';
    cout << ' ' << m[4][2] << ' ';
    cout << ' ' << m[4][3] << ' ';
    cout << ' ' << m[4][4] << ' ';
    cout << ' ' << m[4][5] << "\n";

    // Row 5 – cells 22 23 24 
    cout << "      ";
    cout << ' ' << m[5][2] << ' ';
    cout << ' ' << m[5][3] << ' ';
    cout << ' ' << m[5][4] << "\n";

    // Row 6 – cells 25
    cout << "         ";
    cout << ' ' << m[6][3] << "\n";

    // Numbers under the diamond board (1 to 25)
    cout << "\n";
    cout << "          1        \n";
    cout << "       2  3  4     \n";
    cout << "    5  6  7  8  9  \n";
    cout << " 10 11 12 13 14 15 16 \n";
    cout << "    17 18 19 20 21   \n";
    cout << "       22 23 24      \n";
    cout << "          25         \n";
    cout << "\n";
}
