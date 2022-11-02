#pragma once
#include <cstddef>
#include <string>
#include <list>
using namespace std;

class Move{
    public:
        int nr=0;
        string san;
        string lan;
        string coord;
        string commentary;
        string adnotation;
        Move* previous;
        list<Move*> next;
        bool white = 1;    

        Move(Move* prev=NULL, string coords = "");
};