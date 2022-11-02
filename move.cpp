#include "move.hpp"

Move::Move(Move* prev, string coords){
    previous = prev;
    coord = coords;
    if(prev!= NULL)
        prev->next.push_back(this);
}