#include "menu.hpp"

Menu::~Menu(){
    for(Button* btn: buttons)
        delete btn;
    buttons.clear();
}

void Menu::display(){
    for(auto b: buttons){
        b->display();
    }
}
void Menu::work(){
    for(auto b: buttons)
        if(b->isHovered())
            b->call();
}
std::vector<Button*> Menu::get_buttons(){
    return buttons;
}

void MenuBar::add_button(Button* btn){
    buttons.push_back(btn);
    if(buttons.size()>1){
        btn->move(buttons[buttons.size()-2]->getOffsetX(),0);
        btn->adjust_position();
    }
    else btn->adjust_position();
}

void MenuList::add_button(Button* btn){
    buttons.push_back(btn);
    btn->move(0, buttons.size()*btn->getHeihth());
    btn->setWidth(200);
    btn->adjust_position();
}
void MenuList::move(Vector2i pos){

    for(int i=0;i<buttons.size();i++){
    buttons[i]->resetPosition();
    buttons[i]->move(0, i*buttons[i]->getHeihth());
    buttons[i]->move(pos.x,pos.y);
    buttons[i]->adjust_position();
    }
}
