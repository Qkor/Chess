#include <SFML/Graphics.hpp>

#include "functions.hpp"
#include "board.hpp"

using namespace std;

void set_figure_texture(sf::Sprite &piece,char type, float square_size){
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
    piece.setTextureRect(sf::IntRect(square_size*piece_type,piece_color*square_size,square_size,square_size));
}

void set_position(char board[8][8], sf::Sprite piece[8][8], float square_size){
    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
            set_figure_texture(piece[i][j], board[i][j], square_size);
}


void gui(){

    int board_size = 600;
    float square_size = float(board_size)/8;

    sf::Texture board_texture;
    board_texture.loadFromFile("board.png");
    sf::Sprite chessboard(board_texture);

    sf::Texture pieces_texture;
    pieces_texture.loadFromFile("pieces.png");

    sf::Sprite pieces[8][8];
    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++){
            pieces[i][j].setTexture(pieces_texture);
            pieces[i][j].setPosition(square_size*j,square_size*i);
        }

    sf::RenderWindow window;
    window.create(sf::VideoMode(board_size, board_size), "Chess", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
    
    sf::Event event;

    bool dragging=0;
    int piece_dragged_x, piece_dragged_y;
    int target_x, target_y;
    sf::Sprite piece_dragged;
    string mov="";
    piece_dragged.setTexture(pieces_texture);


    sf::RectangleShape popup(sf::Vector2f(square_size*4,square_size));
    popup.setFillColor(sf::Color(200, 200, 220));
    popup.setPosition(2*square_size,3.5*square_size);
    sf::Sprite figures_to_choose[4];
    for(int i=0;i < 4;i++){
        figures_to_choose[i].setTexture(pieces_texture);
        char type;
        switch (i) {
            case 0: type='Q';break;
            case 1: type='R';break;
            case 2: type='B';break;
            case 3: type='N';break;
        }
        set_figure_texture(figures_to_choose[i],type,square_size);
        figures_to_choose[i].setPosition(square_size*i+popup.getPosition().x,popup.getPosition().y);
    }
    bool choosing_figure=0;
    bool is_figure_chosen=0;
    char figure_chosen;


    Position position;
    char board[8][8];
    position.get_position(board);

    set_position(board, pieces, square_size);

    while (window.isOpen()){

        sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);

        while (window.pollEvent(event)){
            if(event.type == sf::Event::Closed)
                 window.close();

            if(event.type == sf::Event::KeyPressed){
                if(event.key.code==sf::Keyboard::N){
                    position.new_game();
                    position.get_position(board);
                    set_position(board, pieces, square_size);
                }
            }


            if(event.type == sf::Event::MouseButtonPressed){
                if(choosing_figure){
                    for(int i=0;i<4;i++)
                        if(figures_to_choose[i].getGlobalBounds().contains(mouse_pos.x,mouse_pos.y)){
                            switch (i) {
                                case 0: figure_chosen='Q';break;
                                case 1: figure_chosen='R';break;
                                case 2: figure_chosen='B';break;
                                case 3: figure_chosen='N';break;
                            }
                            if(mov[2]==7)
                                figure_chosen+=32;
                            choosing_figure = 0;
                            mov+=figure_chosen;
                            position.move(mov);
                            position.get_position(board);
                            set_position(board, pieces, square_size);
                        }
                            
                }
                else
                    for(int i=0;i<8;i++)
                        for(int j=0;j<8;j++){
                            if(board[i][j]!='.')
                                if(pieces[i][j].getGlobalBounds().contains(mouse_pos.x,mouse_pos.y)){
                                    piece_dragged_x = i;
                                    piece_dragged_y = j;
                                    dragging = 1;
                                }
                        }
            }
                
            if(event.type == sf::Event::MouseButtonReleased){
                for(int i=0;i<8;i++)
                    for(int j=0;j<8;j++){
                        if(pieces[i][j].getGlobalBounds().contains(mouse_pos.x,mouse_pos.y)){
                            target_x = i;
                            target_y = j;
                        }
                    }
                dragging=0;
                mov="";
                mov+=char(piece_dragged_x);
                mov+=char(piece_dragged_y);
                mov+=char(target_x);
                mov+=char(target_y);

                if((board[piece_dragged_x][piece_dragged_y] == 'P' && target_x == 0)||(board[piece_dragged_x][piece_dragged_y] == 'p' && target_x == 7)){
                    choosing_figure=1;
                }

                if(choosing_figure == 0){
                    position.move(mov);
                    position.get_position(board);
                    set_position(board, pieces, square_size);
                }
                
            }

        }

        if(dragging){
            piece_dragged.setTextureRect(pieces[piece_dragged_x][piece_dragged_y].getTextureRect());
            piece_dragged.setPosition(mouse_pos.x-square_size/2,mouse_pos.y-square_size/2);
        }
    
        window.clear();
        window.draw(chessboard);
        for(int i=0;i<8;i++)
            for(int j=0;j<8;j++){
                if(dragging && i == piece_dragged_x && j == piece_dragged_y)
                    continue;
                window.draw(pieces[i][j]);
            }

        if(dragging)
            window.draw(piece_dragged);
        if(choosing_figure){
            window.draw(popup);
            for(int i=0;i<4;i++)
                window.draw(figures_to_choose[i]);
        }
        
        window.display();
    }
}


int main(){
    gui();
}