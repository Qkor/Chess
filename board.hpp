#pragma once
#include <string>

using namespace std;

class Position{

    char board[8][8];
    bool white_on_move;
    bool white_short_castle;
    bool white_long_castle;
    bool black_short_castle;
    bool black_long_castle;
    string en_passant;

    bool check_bishop_move(string mov);
    bool check_rook_move(string mov);
    bool check_queen_move(string mov);
    bool check_knight_move(string mov);
    bool check_king_move(string mov);
    bool check_pawn_move(string mov);
    bool check_move(string mov);

public:

    bool is_position_legal();
    bool is_move_legal(string mov);

    bool king_checked(bool white);
    void clear_board();
    void set_piece(char piece, char x, char y);
    void fen2position(string fen);
    void new_game();
    void make_move(string mov);
    bool move(string mov);
    char** get_position();

    Position();
};