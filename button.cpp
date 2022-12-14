#include "button.hpp"

Button::Button(RenderWindow* window, std::string txt, void (*callback)(), unsigned int font_size) {
    this->window = window;
    this->callback = callback;
    this->font_size = font_size;
    font.loadFromFile("font.ttf");
    text.setFont(font);
    text.setCharacterSize(font_size);
    text.setString(txt);
    background.setSize(Vector2f(text.getLocalBounds().width+font_size,font_size*1.25));
    color = Color(50,50,50);
}

Button::~Button(){
    callback = NULL;
}

void Button::resetPosition(){
    text.setPosition(0,0);
}
int Button::getOffsetX(){
    return background.getPosition().x+background.getLocalBounds().width;
}
int Button::getHeihth(){
    return font_size*1.25;
}
int Button::getWidth(){
    return background.getLocalBounds().width;
}
void Button::setWidth(float width){
    background.setSize(Vector2f(width, background.getLocalBounds().height));
}
void Button::move(int offsetX,int offsetY){
    text.move(offsetX,offsetY);
    background.setPosition(text.getPosition());
}
void Button::adjust_position(){
    text.move(float(font_size)/2,0);
}
void Button::draw(){
    window->draw(background);
    window->draw(text);
}
bool Button::isHovered(){
    Vector2i mouse_position = Mouse::getPosition(*window);
    return background.getGlobalBounds().contains(mouse_position.x,mouse_position.y);
}
void Button::display(){
    draw();
    if(isHovered())
        background.setFillColor(Color(color.r+50,color.g+50,color.b+50));
    else 
        background.setFillColor(color);
}
void Button::call(){
    callback();
}
void Button::set_text(std::string text){
    this->text.setString(text);
}
void Button::set_color(Color color){
    this->color = color;
}