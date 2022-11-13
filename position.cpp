#include "position.hpp"
#include "move.hpp"
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

Position::Position(){
    start_position();
}

bool Position::check_bishop_move(string mov){
    if(abs(mov[0]-mov[2])!=abs(mov[1]-mov[3]))
        return 0;
    char j = min(mov[0],mov[2]) + 1;
    if((mov[0]<mov[2]&&mov[1]<mov[3])||(mov[2]<mov[0]&&mov[3]<mov[1]))
        for(char i = min(mov[1],mov[3]) + 1;i < max(mov[1],mov[3]);++i){
            if(board[j][i] != '.')
                return 0;
            j++;
        }
    else
        for(char i = max(mov[1],mov[3]) - 1;i > min(mov[1],mov[3]);--i){
            if(board[j][i] != '.')
                return 0;
            j++;
        }
    return 1;
}
bool Position::check_rook_move(string mov){
    if(mov[0]==mov[2]){
        for(char i = min(mov[1],mov[3]) + 1; i < max(mov[1],mov[3]); i++)
            if(board[mov[0]][i]!='.')
                return 0;
        return 1;
    }
    if(mov[1]==mov[3]){
        for(char i = min(mov[0],mov[2]) + 1; i < max(mov[0],mov[2]); i++)
            if(board[i][mov[1]]!='.')
                return 0;
        return 1;
    }
    return 0;
}
bool Position::check_queen_move(string mov){
    return check_bishop_move(mov) || check_rook_move(mov);
}
bool Position::check_knight_move(string mov){
    if(abs(mov[0]-mov[2])==2&&abs(mov[1]-mov[3])==1)
        return 1;
    if(abs(mov[0]-mov[2])==1&&abs(mov[1]-mov[3])==2)
        return 1;
    return 0;
}
bool Position::check_king_move(string mov){
    
    if(abs(mov[0]-mov[2]) <= 1 && abs(mov[1]-mov[3]) <= 1)
        return 1;
    
    // castle  
    bool white = board[mov[0]][mov[1]] == 'K';

    if(king_checked(white))
        return 0;
    char rook;
    char line;
    bool short_castle;
    bool long_castle;
    if(white){
        short_castle = white_short_castle;
        long_castle = white_long_castle;
        line = 7;
        rook = 'R';
    }    
    else{
        short_castle = black_short_castle;
        long_castle = black_long_castle;
        line = 0;
        rook = 'r';
    }
    if(mov[0]==line && mov[1]==4){
        if(mov[2]==line && mov[3]==6 && short_castle){
            if(board[line][6]!='.' || board[line][5]!='.' || board[line][7]!=rook)
                return 0;
            mov[3]=5;
            if(!is_move_legal(mov))
                return 0;
            return 1;
        }
        else if(mov[2]==line && mov[3]==2 && long_castle){
            if(board[line][2]!='.' || board[line][3]!='.' || board[line][0]!=rook)
                return 0;
            mov[3]=3;
            if(!is_move_legal(mov))
                return 0;
            return 1;
        }
    }
    return 0;
}
bool Position::check_pawn_move(string mov){
    
    bool white = board[mov[0]][mov[1]] == 'P';

    if(white){
        if(mov[3]==en_passant[0]-97&&mov[2]==56-en_passant[1]){
            if(mov[0]==3&&abs(mov[1]-mov[3])==1)
                return 1;
            return 0;
        }
        if(abs(mov[1]-mov[3])==1&&mov[0]==mov[2]+1&&board[mov[2]][mov[3]]>='a'&&board[mov[2]][mov[3]]<='z')
            return 1;
        if(mov[1]==mov[3]&&board[mov[2]][mov[3]]=='.'){
            if(mov[0]==6&&mov[2]==4&&board[5][mov[1]]=='.')
                return 1;
            if(mov[0]==mov[2]+1)
                return 1;
        }
        return 0;
    }
    else{
        if(mov[3]==en_passant[0]-97&&mov[2]==56-en_passant[1]){
            if(mov[0]==4&&abs(mov[1]-mov[3])==1)
                return 1;
            return 0;
        }
        if(abs(mov[1]-mov[3])==1&&mov[0]+1==mov[2]&&board[mov[2]][mov[3]]>='A'&&board[mov[2]][mov[3]]<='Z')
            return 1;
        if(mov[1]==mov[3]&&board[mov[2]][mov[3]]=='.'){
            if(mov[0]==1&&mov[2]==3&&board[2][mov[1]]=='.')
                return 1;
            if(mov[0]+1==mov[2])
                return 1;
        }
        return 0;
    }
}
bool Position::check_move(string mov){
    
    char fig = board[mov[0]][mov[1]];
    char target = board[mov[2]][mov[3]];
    bool white = fig<91;
    
    if(!white && target>='a' && target<='z')
        return 0;
    if(white && target>='A' && target<='Z')
        return 0;

    if(white)
        fig+=32;

    switch(fig){
        case 'b':
            return check_bishop_move(mov);
            break;
        case 'r':
            return check_rook_move(mov);
            break;
        case 'q':
            return check_queen_move(mov);
            break;
        case 'n':
            return check_knight_move(mov);
            break;
        case 'k':
            return check_king_move(mov);
            break;
        case 'p':
            return check_pawn_move(mov); 
            break;     
    }
    
    return 0;
}
bool Position::king_checked(bool white=1){
    
    char king;
    char king_x;
    char king_y;
    char figures_start;
    char figures_end;

    if(white){
        king = 'K';
        figures_start = 'a';
        figures_end = 'z';
    }
    else{
        king = 'k'; 
        figures_start = 'A';
        figures_end = 'Z';
    }    

    bool king_found = 0;
    for(char i=0;i<8;i++){
        if(king_found)
            break;
        for(char j=0;j<8;j++)
            if(board[i][j]==king){
                king_x = i;
                king_y = j;
                king_found = 1;
                break;
            }
    }
    
    for(char i=0;i<8;i++)
        for(char j=0;j<8;j++)
            if(board[i][j] >= figures_start && board[i][j] <= figures_end){
                string mov = "";
                mov+=i;
                mov+=j;
                mov+=king_x;
                mov+=king_y;
                if(board[i][j]=='k' || board[i][j]=='K'){
                    if(abs(mov[0]-mov[2]) <= 1 && abs(mov[1]-mov[3]) <= 1)
                        return 1;
                }
                else if(check_move(mov))
                    return 1;
            }
    return 0;
}

bool Position::is_position_legal(){
    for(int i=0;i<8;i++)
        if(board[0][i]=='p'||board[0][i]=='P'||board[7][i]=='p'|board[7][i]=='P')
            return 0;
    
    char white_kings = 0;
    char black_kings = 0;
    for(char i=0;i<8;i++)
        for(char j=0;j<8;j++){
            if(board[i][j]=='K')
                white_kings++;
            else if (board[i][j]=='k')
                black_kings++;
        }
    if(white_kings != 1 || black_kings != 1)
        return 0;

    if(king_checked(1) && !white_on_move)
        return 0;
    if(king_checked(0) && white_on_move)
        return 0;

    return 1;
}
bool Position::is_move_legal(string mov){
    
    for(int i=0;i<4;i++)
        if(mov[i]<0||mov[i]>7)
            return 0;
    if(mov[0]==mov[2]&&mov[1]==mov[3])
        return 0;
    
    if(board[mov[0]][mov[1]]=='.')
        return 0;
    bool white_figure = board[mov[0]][mov[1]] < 91;
    if(white_figure && !white_on_move)
        return 0;
    if(!white_figure && white_on_move)
        return 0;
    if(!check_move(mov))
        return 0;

    Position new_position;
    new_position = *this;
    new_position.make_move(mov);
    return new_position.is_position_legal();
}

void Position::make_move(string mov){         //no legality check

    if(mov.size()==5)
        board[mov[2]][mov[3]]=mov[4];
    else
        board[mov[2]][mov[3]]=board[mov[0]][mov[1]];
    board[mov[0]][mov[1]] = '.';

    if(board[mov[2]][mov[3]]=='k'){
        black_long_castle = 0;
        black_short_castle = 0;
    }
    if(board[mov[2]][mov[3]]=='K'){
        white_long_castle = 0;
        white_short_castle = 0;
    }

    // castles

    if(board[mov[2]][mov[3]]=='k' && mov[0]==0 && mov[1]==4){                         // black
        if(mov[2]==0 && mov[3]==6){         // 0-0
            board[0][5]='r';
            board[0][7]='.';
            black_long_castle = 0;
            black_short_castle = 0;
        }
        else if(mov[2]==0 && mov[3]==2){    // 0-0-0
            board[0][3]='r';
            board[0][0]='.';
            black_long_castle = 0;
            black_short_castle = 0;
        }
    }
    else if(board[mov[2]][mov[3]]=='K' && mov[0]==7&&mov[1]==4){                         // white
        if(mov[2]==7 && mov[3]==6){         // 0-0
            board[7][5]='R';
            board[7][7]='.';
            white_long_castle = 0;
            white_short_castle = 0;
        }
        else if(mov[2]==7 && mov[3]==2){    // 0-0-0
            board[7][3]='R';
            board[7][0]='.';
            white_long_castle = 0;
            white_short_castle = 0;
        }
    }

    if(black_long_castle && board[0][0]!='r')
        black_long_castle = 0;
    if(black_short_castle && board[0][7]!='r')
        black_short_castle = 0;   
    if(white_long_castle && board[7][0]!='R')
        white_long_castle = 0;
    if(white_short_castle && board[7][7]!='R')
        white_short_castle = 0; 

    // en passant

    if(board[mov[2]][mov[3]]=='P'&&56-en_passant[1]==mov[2]&&en_passant[0]-97==mov[3]){   
        board[mov[2]+1][mov[3]]='.';
    }
    else if(board[mov[2]][mov[3]]=='p'&&56-en_passant[1]==mov[2]&&en_passant[0]-97==mov[3]){
        board[mov[2]-1][mov[3]]='.';
    }

    if(board[mov[2]][mov[3]]=='P'&&mov[0]-mov[2]==2){
        en_passant="";
        en_passant+=char(mov[3]+97);
        en_passant+=char(55-mov[2]);
    }
    else if(board[mov[2]][mov[3]]=='p'&&mov[2]-mov[0]==2){
        en_passant="";
        en_passant+=char(mov[3]+97);
        en_passant+=char(57-mov[2]);
    }
    else en_passant="-";

    white_on_move = !white_on_move;
}
bool Position::move(string mov){
   if(is_move_legal(mov)){
        make_move(mov);
        return 1;
   }
    return 0;
}

void Position::clear_board(){
    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
            board[i][j]='.';
    white_short_castle=false;
    white_long_castle=false;
    black_short_castle=false;
    black_long_castle=false;
}
void Position::set_piece(char piece, char x, char y){
    board[x][y] = piece;
}
void Position::fen2position(string fen){
    clear_board();
    int f=0;
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            if(fen[f]<=56&&fen[f]>48)
                j+=fen[f]-49;
            else if(fen[f]=='/')
                j--;
            else
                board[i][j]=fen[f];
            f++;
        }
    }
    f++;
    white_on_move = fen[f]=='w';
    f++;f++;
    if(fen[f]=='K'){
        white_short_castle=true;
        f++;
    }
    if(fen[f]=='Q'){
        white_long_castle=true;
        f++;
    }
    if(fen[f]=='k'){
        black_short_castle=true;
        f++;
    }
    if(fen[f]=='q'){
        black_long_castle=true;
        f++;
    }
    if(fen[f]=='-')
        f++;
    f++;
    set_en_passant(fen.substr(f,2));
}
string Position::position2fen(){
    string fen="";
    for(int i=0;i<8;i++){
        char x = '0';
        for(int j=0;j<8;j++){
            if(board[i][j]=='.')
                x++;
            else if(x>'0'){
                fen+=x;
                fen+=board[i][j];
                x='0';
            }
            else
                fen+=board[i][j];
        }
        if(x>'0'){
            fen+=x;
            x='0';
        }
        if(i!=7)
            fen+='/';
    } 
    fen+=' ';
    fen+= is_white_on_move()?'w':'b';
    fen+=' ';
    if(white_short_castle)
        fen+='K';
    if(white_long_castle)
        fen+='Q';
    if(black_short_castle)
        fen+='k';
    if(black_long_castle)
        fen+='q';
    if(!black_short_castle&&!black_long_castle&&!white_long_castle&&!white_short_castle)
        fen+='-';
    fen+=' ';
    fen+=en_passant;
    return fen;
}
void Position::start_position(){
    fen2position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

char** Position::get_position(){
    char** bd;
    bd = new char*[8];
    for(int i=0;i<8;i++){
        bd[i]=new char[8];
        for(int j=0;j<8;j++)
            bd[i][j]=board[i][j];
    }
    return bd;
}

bool Position::is_white_on_move(){
    return white_on_move;
}
void Position::switch_side_to_move(){
    white_on_move = !white_on_move;
}

bool Position::get_white_long_castle(){
    return white_long_castle;
}
bool Position::get_white_short_castle(){
    return white_short_castle;
}
bool Position::get_black_long_castle(){
    return black_long_castle;
}
bool Position::get_black_short_castle(){
    return black_short_castle;
}
void Position::switch_white_long_castle(){
    white_long_castle = !white_long_castle;
}
void Position::switch_white_short_castle(){
    white_short_castle = !white_short_castle;
}
void Position::switch_black_long_castle(){
    black_long_castle = !black_long_castle;
}
void Position::switch_black_short_castle(){
    black_short_castle = !black_short_castle;
}
void Position::set_en_passant(string square){
    if(square[0]=='-')
        en_passant="-";
    en_passant = square;
}

/*
vector<tuple<char,char,char>> Position::find_all_pieces(){
    vector<tuple<char,char,char>> pieces;
    char start,end;
    if(is_white_on_move()){
        start='A';
        end='Z';
    }
    else{
        start='a';
        end='z';
    }
    for(char i=0;i<8;i++)
        for(char j=0;i<8;i++)
            if(board[i][j]>=start && board[i][j]<=end)
                pieces.push_back(make_tuple(board[i][j],i,j));
    return pieces;
}

vector<string> Position::find_all_moves(){
    vector<tuple<char,char,char>> pieces = find_all_pieces();
    for(auto piece: pieces){
        switch(get<0>(piece)){
            
        }
    } 
}*/