#include "laser.hpp"
#include <iostream>

Laser::Laser(Vector2 position, int speed)
{
    this->position = position;
    this->speed = speed;
    active = true;
}

void Laser::Draw() 
{
    if (active)
        DrawRectangle(position.x, position.y, 5, 20, {243, 216, 63, 255});
}

void Laser::Update() 
{
    position.y += speed;
    if(active) {
        if (position.y < 25 || position.y > GetScreenHeight() - 100) {
            active = false; // Deactivate laser if it goes out of bounds
            // std::cout << "Laser Inactive" << std::endl;
        }
    }
}

Rectangle Laser::getRect() 
{
    Rectangle rect;
    rect.x = position.x;
    rect.y = position.y;
    rect.width = 5;
    rect.height = 20;
    return rect;
}