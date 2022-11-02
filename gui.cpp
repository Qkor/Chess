#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <vector>
#include "functions.hpp"
#include "board.hpp"
#include "game.hpp"
#include "menu.hpp"
using namespace std;
using namespace sf;

class Gui{

    int board_size;
    float square_size;
    int offset;
    Texture board_texture;
    Texture pieces_texture;
    Game game;
    char** board;
    Sprite chessboard;
    Sprite pieces[8][8];

    bool promoting;
    RectangleShape popup;
    Sprite pieces_to_choose[4];

    bool dragging;
    string mov;
    Vector2i piece_dragged_index;
    Vector2i mouse_pos;
    Vector2i target;
    Sprite piece_dragged;
    
    RenderWindow window;
    Event event;

    MenuBar menu;
    MenuList menu_list;
    bool show_menu_list = 0;
    bool setup_mode = 0;
    char piece_to_set='.';

  public:

    Gui(){
        offset = 30;
        board_size = 600;
        square_size = float(board_size)/8;

        board_texture.loadFromFile("board.png");
        pieces_texture.loadFromFile("pieces.png");

        chessboard.setTexture(board_texture);
        chessboard.setPosition(0,offset);
        piece_dragged.setTexture(pieces_texture);

        for(int i=0;i<8;i++)
            for(int j=0;j<8;j++){
                pieces[i][j].setTexture(pieces_texture);
                pieces[i][j].setPosition(square_size*j,square_size*i+offset);
            }
        
        dragging = 0;

        promoting = 0;
        popup.setSize(Vector2f(4*square_size,square_size));
        popup.setFillColor(Color(140,130,120,230));
        popup.setPosition(square_size*2,square_size*3.5+offset);
        for(int i=0;i<4;i++){
            pieces_to_choose[i].setTexture(pieces_texture);
            pieces_to_choose[i].setPosition(i*square_size+popup.getPosition().x,popup.getPosition().y);
        }

        board = game.get_position();
        set_position();

        window.create(VideoMode(board_size, board_size + offset), "Chess", Style::Titlebar | Style::Close);
        window.setFramerateLimit(60);

        menu.add_button(new Button(&window, "new game"));
        menu.add_button(new Button(&window, "undo move"));
        menu.add_button(new Button(&window, "setup position"));
        
        menu_list.add_button(new Button(&window, "king"));
        menu_list.add_button(new Button(&window, "queen"));
        menu_list.add_button(new Button(&window, "rook"));
        menu_list.add_button(new Button(&window, "bishop"));
        menu_list.add_button(new Button(&window, "knight"));
        menu_list.add_button(new Button(&window, "pawn"));
        menu_list.add_button(new Button(&window, "clear board"));
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
        board = game.get_position();
        set_position();
    }

    void display_board(){
         window.clear(Color(50,50,50));
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
            if(promoting)
                display_popup();
            menu.display();
            if(show_menu_list)
                menu_list.display();

            window.display();
    }

    void display_popup(){
        
        int adjust_color=0;
        if(board[piece_dragged_index.x][piece_dragged_index.y]=='p')
            adjust_color=32;

        set_piece_texture(pieces_to_choose[0], 'Q'+adjust_color);
        set_piece_texture(pieces_to_choose[1], 'R'+adjust_color);
        set_piece_texture(pieces_to_choose[2], 'B'+adjust_color);
        set_piece_texture(pieces_to_choose[3], 'N'+adjust_color);

        window.draw(popup);
        for(int i=0;i<4;i++)
            window.draw(pieces_to_choose[i]);
        
    }

    void handle_input(){
        while (window.pollEvent(event)){

            mouse_pos = Mouse::getPosition(window);
            
            if(event.type == Event::Closed)
                window.close();

            if(event.type == Event::KeyPressed){
                if(event.key.code==Keyboard::N){
                    game.new_game();
                    refresh_position();
                }
                if(event.key.code==Keyboard::Left){
                    game.move_back();
                    refresh_position();
                }
                if(event.key.code==Keyboard::Right){
                    game.move_forward();
                    refresh_position();
                }
            }
            if(event.type == Event::MouseButtonPressed && !show_menu_list){

                if(event.mouseButton.button == sf::Mouse::Right && setup_mode){
                    show_menu_list = true;
                    menu_list.move(mouse_pos);
                }

                else if(event.mouseButton.button == sf::Mouse::Left){

                    if(promoting){
                        char piece_chosen=' ';
                        if(pieces_to_choose[0].getGlobalBounds().contains(mouse_pos.x,mouse_pos.y))
                            piece_chosen='Q';
                        else if (pieces_to_choose[1].getGlobalBounds().contains(mouse_pos.x,mouse_pos.y))
                            piece_chosen='R';
                        else if (pieces_to_choose[2].getGlobalBounds().contains(mouse_pos.x,mouse_pos.y))
                            piece_chosen='B';
                        else if (pieces_to_choose[3].getGlobalBounds().contains(mouse_pos.x,mouse_pos.y))
                            piece_chosen='N';

                        if(piece_chosen != ' '){
                            if(board[piece_dragged_index.x][piece_dragged_index.y]=='p')
                                piece_chosen+=32;
                            move(piece_chosen);
                            promoting=0;
                        }
                    }
                    else{
                        if(setup_mode){
                            for(int i=0;i<8;i++)
                                for(int j=0;j<8;j++)
                                    if(pieces[i][j].getGlobalBounds().contains(mouse_pos.x,mouse_pos.y)){
                                        if(board[i][j] == piece_to_set)
                                            game.set_piece(piece_to_set+32, i, j);
                                        else if(board[i][j] == piece_to_set+32)
                                            game.set_piece('.',i,j);
                                        else
                                            game.set_piece(piece_to_set, i, j);
                                        refresh_position();
                                        break;
                                    }
                        }
                        else{
                            for(int i=0;i<8;i++)
                                for(int j=0;j<8;j++)
                                    if(board[i][j]!='.')
                                        if(pieces[i][j].getGlobalBounds().contains(mouse_pos.x,mouse_pos.y)){
                                            piece_dragged_index.x = i;
                                            piece_dragged_index.y = j;
                                            dragging = 1;
                                        }
                        }
                    }
                        
                }
                
            }
            else if(dragging && event.type == Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left &&!promoting){
                dragging=0;
                for(int i=0;i<8;i++)
                    for(int j=0;j<8;j++)
                        if(pieces[i][j].getGlobalBounds().contains(mouse_pos.x,mouse_pos.y)){
                            target.x = i;
                            target.y = j;
                            if((board[piece_dragged_index.x][piece_dragged_index.y] == 'P' && target.x == 0)||(board[piece_dragged_index.x][piece_dragged_index.y] == 'p' && target.x == 7))
                                promoting = 1;
                            else
                                move();
                        }
            }
            else if(event.type == Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left &&!promoting){
                std::vector<Button*> buttons = menu.get_buttons();
                if(buttons[0]->isHovered()){
                    game.new_game();
                    setup_mode = 0;
                    refresh_position();
                }
                else if(buttons[1]->isHovered()){
                    game.move_back();
                    refresh_position();
                }
                else if(buttons[2]->isHovered()){
                    setup_mode = !setup_mode;
                }
                if(show_menu_list){
                    std::vector<Button*> buttons = menu_list.get_buttons();
                    if(buttons[0]->isHovered())
                        piece_to_set = 'K';
                    if(buttons[1]->isHovered())
                        piece_to_set = 'Q';
                    if(buttons[2]->isHovered())
                        piece_to_set = 'R';
                    if(buttons[3]->isHovered())
                        piece_to_set = 'B';
                    if(buttons[4]->isHovered())
                        piece_to_set = 'N';
                    if(buttons[5]->isHovered())
                        piece_to_set = 'P';
                    if(buttons[6]->isHovered()){
                        game.clear_board();
                        refresh_position();
                    }
                    show_menu_list = false;
                }
            }
        }
    }
    void move(char promoted_piece = ' '){
        mov="";
        mov+=piece_dragged_index.x;
        mov+=piece_dragged_index.y;
        mov+=target.x;
        mov+=target.y;
        if(promoting)
            mov+=promoted_piece;
        game.add_move(mov);
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