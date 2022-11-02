#pragma once
#include <vector>
#include "button.hpp"

class Menu{
  protected:
    std::vector<Button*> buttons;
  public:
    void add_button(Button* btn);
    void display();
    void work();
    std::vector<Button*> get_buttons();
};

class MenuBar : public Menu{
  public:
    void add_button(Button* btn);
};

class MenuList : public Menu{
  public:
    void add_button(Button* btn);
    void move(Vector2i pos);
};