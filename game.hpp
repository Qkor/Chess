#pragma once
#include <list>
#include <map>
#include <stack>
#include <string>
#include "position.hpp"
#include "move.hpp"


class Game : public Position{
    protected:
        map<string,string> tags;
        Move* first_move;
        Move* current_move;
        string starting_position = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    public:
        Game();
        ~Game();
        void move(Move* mov);
        void add_move(string coords);
        void move_back();
        void move_forward(int variation_nr=0);
        void delete_variation(Move* move);
        void set_piece(char piece, char x, char y);
        void new_game();
        void set_position();
};