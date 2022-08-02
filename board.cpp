#include "board.hpp"
#include <iostream>

using namespace std;


bool Position::check_bishop_move(char board[8][8], string mov){
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
bool Position::check_rook_move(char board[8][8], string mov){
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
bool Position::check_queen_move(char board[8][8], string mov){
    return check_bishop_move(board, mov) || check_rook_move(board, mov);
}
bool Position::check_knight_move(char board[8][8], string mov){
    if(abs(mov[0]-mov[2])==2&&abs(mov[1]-mov[3])==1)
        return 1;
    if(abs(mov[0]-mov[2])==1&&abs(mov[1]-mov[3])==2)
        return 1;
    return 0;
}
bool Position::check_king_move(char board[8][8], string mov, bool short_castle = 1, bool long_castle = 1){
    
    bool white = board[mov[0]][mov[1]] == 'K';

    // castle  ////////////////////////
    char rook;
    char line;
    if(white){
        line = 7;
        rook = 'R';
    }    
    else{
        line = 0;
        rook = 'r';
    }
    if(mov[0]==line && mov[1]==4){
        if(mov[2]==line && mov[3]==6 && short_castle){
            if(board[line][5]!='.'||board[line][7]!=rook)
                return 0;
            return 1;
        }
        else if(mov[2]==line && mov[3]==2 && long_castle){
            if(board[line][3]!='.'||board[line][0]!=rook)
                return 0;
            return 1;
        }
    }
    ///////////////////////////////////
    
    if(abs(mov[0]-mov[2])>1 || abs(mov[1]-mov[3])>1)
        return 0;
    return 1;
}
bool Position::check_pawn_move(char board[8][8], string mov, string en_passant="-"){
    
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
bool Position::check_move(Position position, string mov){
    
    for(int i=0;i<4;i++)
        if(mov[i]<0||mov[i]>7)
            return 0;
    if(mov[0]==mov[2]&&mov[1]==mov[3])
        return 0;
    
    char fig = position.board[mov[0]][mov[1]];
    char target = position.board[mov[2]][mov[3]];
    bool white = fig<91;
    
    if(!white && target>='a' && target<='z')
        return 0;
    if(white && target>='A' && target<='Z')
        return 0;

    if(white)
        fig+=32;

    switch(fig){
        case 'b':
            return check_bishop_move(position.board, mov);
            break;
        case 'r':
            return check_rook_move(position.board, mov);
            break;
        case 'q':
            return check_queen_move(position.board, mov);
            break;
        case 'n':
            return check_knight_move(position.board, mov);
            break;
        case 'k':
            if(white)
                return check_king_move(position.board, mov, position.white_short_castle, position.white_long_castle);
            else
                return check_king_move(position.board, mov, position.black_short_castle, position.black_long_castle);
            break;
        case 'p':
            return check_pawn_move(position.board, mov, position.en_passant); 
            break;     
    }
    
    return 0;
}
bool Position::king_checked(Position position, bool white=1){
    
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
            if(position.board[i][j]==king){
                king_x = i;
                king_y = j;
                king_found = 1;
                break;
            }
    }
    
    for(char i=0;i<8;i++)
        for(char j=0;j<8;j++)
            if(position.board[i][j] >= figures_start && position.board[i][j] <= figures_end){
                string mov = "";
                mov+=i;
                mov+=j;
                mov+=king_x;
                mov+=king_y;
                if(check_move(position, mov))
                    return 1;
            }
    return 0;
}

bool Position::is_position_legal(Position position){
    for(int i=0;i<8;i++)
        if(position.board[0][i]=='p'||position.board[0][i]=='P'||position.board[7][i]=='p'||position.board[7][i]=='P')
            return 0;
    
    char white_kings = 0;
    char black_kings = 0;
    for(char i=0;i<8;i++)
        for(char j=0;j<8;j++){
            if(position.board[i][j]=='K')
                white_kings++;
            else if (position.board[i][j]=='k')
                black_kings++;
        }
    if(white_kings != 1 || black_kings != 1)
        return 0;

    if(king_checked(position,1) && !position.white_on_move)
        return 0;
    if(king_checked(position,0) && position.white_on_move)
        return 0;

    return 1;
}
bool Position::is_move_legal(Position position, string mov){
    if(position.board[mov[0]][mov[1]]=='.')
        return 0;
    bool white_figure = position.board[mov[0]][mov[1]] < 91;
    if(white_figure && !position.white_on_move)
        return 0;
    if(!white_figure && position.white_on_move)
        return 0;
    if(!check_move(position,mov))
        return 0;

    Position new_position;
    new_position = position;
    new_position.make_move(mov);
    return is_position_legal(new_position);
}

void Position::make_move(string mov){         //no legality check

    if(mov.size()==5)
        board[mov[2]][mov[3]]=mov[4];
    else
        board[mov[2]][mov[3]]=board[mov[0]][mov[1]];
    board[mov[0]][mov[1]] = '.';

    // castles

    if(board[mov[2]][mov[3]]=='k'){                         // black
        if(mov[0]==0&&mov[1]==4&&mov[2]==0&&mov[3]==6){         // 0-0
            board[0][5]='r';
            board[0][7]='.';
        }
        else if(mov[0]==0&&mov[1]==4&&mov[2]==0&&mov[3]==2){    // 0-0-0
            board[0][3]='r';
            board[0][0]='.';
        }
    }
    if(board[mov[2]][mov[3]]=='K'){                         // white
        if(mov[0]==7&&mov[1]==4&&mov[2]==7&&mov[3]==6){         // 0-0
            board[7][5]='R';
            board[7][7]='.';
        }
        else if(mov[0]==7&&mov[1]==4&&mov[2]==7&&mov[3]==2){    // 0-0-0
            board[7][3]='R';
            board[7][0]='.';
        }
    }

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
   if(is_move_legal(*this,mov)){
        make_move(mov);
        return 1;
   }
    return 0;
}

void Position::clear_board(){
    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
            board[i][j]='.';
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
    //properties(fen.substr(f));
}
void Position::new_game(){
    fen2position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    white_short_castle = 1;
    white_long_castle = 1;
    black_short_castle = 1;
    black_long_castle = 1;
    en_passant = "-";
    white_on_move = 1;
}

void Position::get_position(char result[8][8]){
    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
            result[i][j] = board[i][j]; 
}

Position::Position(){
    new_game();
}
