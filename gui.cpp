#include <SFML/Graphics.hpp>
#include "functions.hpp"
#include "board.hpp"

using namespace std;
using namespace sf;

class Gui{

    int board_size;
    float square_size;
    Texture board_texture;
    Texture pieces_texture;
    Position position;
    char** board;
    Sprite chessboard;
    Sprite pieces[8][8];

    bool dragging;
    Vector2i piece_dragged_index;
    Vector2i mouse_pos;
    Vector2i target;
    Sprite piece_dragged;
    
    RenderWindow window;
    Event event;


public:

    Gui(){
        board_size = 600;
        square_size = float(board_size)/8;

        board_texture.loadFromFile("board.png");
        pieces_texture.loadFromFile("pieces.png");

        chessboard.setTexture(board_texture);
        piece_dragged.setTexture(pieces_texture);

        for(int i=0;i<8;i++)
            for(int j=0;j<8;j++){
                pieces[i][j].setTexture(pieces_texture);
                pieces[i][j].setPosition(square_size*j,square_size*i);
            }
        
        dragging = 0;
        board = position.get_position();
        set_position();

        window.create(VideoMode(board_size, board_size), "Chess", Style::Titlebar | Style::Close);
        window.setFramerateLimit(60);
    }

    ~Gui(){
        for(int i=0;i<8;i++)
            delete [] board[i];
        delete [] board;
    }

    void set_piece_texture(Sprite &piece, char type){
        int piece_type;
        bool piece_color = type >= 'a' ? 0:1;
        if(type<='a')
            type+=32;
        switch (type) {
            case 'q': piece_type = 1; break;
            case 'k': piece_type = 2; break;
            case 'r': piece_type = 3; break;
            case 'n': piece_type = 4; break;
            case 'b': piece_type = 5; break;
            case 'p': piece_type = 6; break;
            default: piece_type = 0; break;
        }
        piece.setTextureRect(IntRect(square_size*piece_type,piece_color*square_size,square_size,square_size));
    }

    void set_position(){
        for(int i=0;i<8;i++)
            for(int j=0;j<8;j++)
                set_piece_texture(pieces[i][j], board[i][j]);
    }

    void refresh_position(){
        board = position.get_position();
        set_position();
    }

    void display_board(){
         window.clear();
            window.draw(chessboard);
            for(int i=0;i<8;i++)
                for(int j=0;j<8;j++){
                    if(dragging && i == piece_dragged_index.x && j == piece_dragged_index.y)
                        continue;
                    window.draw(pieces[i][j]);
                }
            if(dragging){
                piece_dragged.setTextureRect(pieces[piece_dragged_index.x][piece_dragged_index.y].getTextureRect());
                piece_dragged.setPosition(mouse_pos.x-square_size/2,mouse_pos.y-square_size/2);
                window.draw(piece_dragged);
            }
                

            window.display();
    }

    void handle_input(){
        while (window.pollEvent(event)){

            mouse_pos = Mouse::getPosition(window);
            
            if(event.type == Event::Closed)
                window.close();

            if(event.type == Event::KeyPressed){
                if(event.key.code==Keyboard::N){
                    position.new_game();
                    refresh_position();
                }
            }
            if(event.type == Event::MouseButtonPressed){
                for(int i=0;i<8;i++)
                    for(int j=0;j<8;j++){
                        if(board[i][j]!='.')
                            if(pieces[i][j].getGlobalBounds().contains(mouse_pos.x,mouse_pos.y)){
                                piece_dragged_index.x = i;
                                piece_dragged_index.y = j;
                                dragging = 1;
                            }
                    }
            }
            else if(event.type == Event::MouseButtonReleased){
                dragging=0;
                for(int i=0;i<8;i++)
                    for(int j=0;j<8;j++)
                        if(pieces[i][j].getGlobalBounds().contains(mouse_pos.x,mouse_pos.y)){
                            target.x = i;
                            target.y = j;
                            move();
                        }
            }
        }
    }
    void move(){
        string mov="";
        mov+=piece_dragged_index.x;
        mov+=piece_dragged_index.y;
        mov+=target.x;
        mov+=target.y;
        position.move(mov);
        refresh_position();
    }

    void main_loop(){
        while (window.isOpen()){
            handle_input();
            display_board();
        }
    }

};


int main(){
    Gui gui;
    gui.main_loop();
}