#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include "functions.hpp"

using namespace sf;

class Button{
  Text text;
  Font font;
  int font_size;
  RectangleShape background;
  RenderWindow* window;
  void (*callback)();

public:
  Button(RenderWindow* window, std::string txt, void (*callback)()=[](){}, unsigned int font_size=24);
  void resetPosition();
  int getOffsetX();
  int getHeihth();
  int getWidth();
  void set_text(std::string text);
  void setWidth(float width);
  void move(int offsetX,int offsetY);
  void adjust_position();
  void draw();
  bool isHovered();
  void display();
  void set_color(Color color);
  void call();
};