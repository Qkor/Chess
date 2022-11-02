#pragma once
#include <list>
#include <map>
#include <stack>
#include <string>
#include "board.hpp"
#include "move.hpp"


class Game{
    private:
        map<string,string> tags;
        Move* first_move;
        Move* current_move;
        Position chessboard;
        //Position start_position;
    public:
        Game();
        char** get_position();
        void go_to_start();
        void move(Move* mov);
        void add_move(string coords);
        void move_back();
        void move_forward(int variation_nr=0);
        void delete_variation(Move* move);
        void new_game();
        void clear_board();
        void set_piece(char piece, char x, char y);
};