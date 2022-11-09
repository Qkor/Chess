#include "game.hpp"
#include "board.hpp"
#include "move.hpp"
#include <cstddef>
#include <cstdio>
#include <type_traits>

Game::Game(){
    first_move = new Move(NULL, "");
    current_move = first_move;
}
Game::~Game(){
    current_move = NULL;
    delete_variation(first_move);
    delete current_move;
    delete first_move;
}

void Game::move(Move* mov){
    Position::move(mov->coord);
    current_move = mov;
}

void Game::add_move(string coords){
    for (list<Move*>::iterator it=current_move->next.begin(); it != current_move->next.end(); ++it){
        if((*it)->coord==coords){
            move(*it);
            return;
        }
    }
    if(is_move_legal(coords)){
        Move *new_move = new Move(current_move, coords);
        current_move->next.push_back(new_move);
        Position::move(coords);
        current_move = new_move;
    }
}

void Game::move_back(){
    stack<Move*> previous_moves;
    while(current_move!=first_move){
        previous_moves.push(current_move->previous);
        current_move = current_move->previous;
    }
    fen2position(starting_position);
    current_move = first_move;
    while(!previous_moves.empty()){
        move(previous_moves.top());
        previous_moves.pop();
    }
}

void Game::move_forward(int variation_nr){
    if(!current_move->next.empty())
        move(current_move->next.front());
}

void Game::delete_variation(Move* move){
    for (list<Move*>::iterator i=move->next.begin(); i!=move->next.end(); ++i)
        delete_variation(*i);
    move->next.clear();
    if(move->previous!=NULL){
        move = NULL;
        delete move;
    }
        
}
void Game::new_game(){
    current_move = first_move;
    delete_variation(first_move);
    start_position();
    starting_position = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
}

void Game::set_piece(char piece, char x, char y){
    board[x][y] = piece;
    set_position();
}

void Game::set_position(){
    current_move = first_move;
    delete_variation(first_move);
    starting_position = position2fen();
}
