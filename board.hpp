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

    bool check_bishop_move(char board[8][8], string mov);
    bool check_rook_move(char board[8][8], string mov);
    bool check_queen_move(char board[8][8], string mov);
    bool check_knight_move(char board[8][8], string mov);
    bool check_king_move(char board[8][8], string mov, bool short_castle, bool long_castle);
    bool check_pawn_move(char board[8][8], string mov, string en_passant);
    bool check_move(Position position, string mov);

public:

    bool is_position_legal(Position position);
    bool is_move_legal(Position position, string mov);

    bool king_checked(Position position, bool white);
    void clear_board();
    void fen2position(string fen);
    void new_game();
    void make_move(string mov);
    bool move(string mov);
    void get_position(char result[8][8]);

    Position();
};