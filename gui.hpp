#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <vector>
#include "position.hpp"
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
    MenuBar setup_menu;
    MenuList menu_list;
    MenuList castle_menu;
    bool show_castle_menu = 0;
    bool show_menu_list = 0;
    bool setup_mode = 0;
    bool setting_ep = 0;
    char piece_to_set='P';

  public:

    Gui();
    ~Gui();
    void set_piece_texture(Sprite &piece, char type);
    void set_position();
    void refresh_position();
    void display_board();
    void display_popup();
    void handle_input();
    void move(char promoted_piece = ' ');
    void main_loop();
};
