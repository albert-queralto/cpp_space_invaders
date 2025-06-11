#include "spaceship.hpp"
#include <raylib.h>

Spaceship::Spaceship()
{
    image = LoadTexture("assets/images/spaceship.png");
    position.x = (GetScreenWidth() - image.width) / 2.0f;
    position.y = GetScreenHeight() - image.height - 10;
    lastFireTime = 0.0;
}

Spaceship::~Spaceship()
{
    UnloadTexture(image);
}

void Spaceship::Draw()
{
    DrawTextureV(image, position, WHITE);
}

void Spaceship::MoveLeft()
{
    position.x -= 7.0f;
    if (position.x < 0) {
        position.x = 0; // Prevent moving out of bounds
    }
}

void Spaceship::MoveRight()
{
    position.x += 7.0f;
    if (position.x > GetScreenWidth() - image.width) {
        position.x = GetScreenWidth() - image.width; // Prevent moving out of bounds
    }
}

void Spaceship::FireLaser()
{
    if (GetTime() - lastFireTime >= 0.35) {
        lasers.push_back(Laser(Vector2{position.x + image.width / 2.0f - 2.5f, position.y}, -6.0f));
        lastFireTime = GetTime();
    }
    
}

Rectangle Spaceship::getRect()
{
    return {position.x, position.y, float(image.width), float(image.height)};
}

void Spaceship::Reset()
{
    position.x = (GetScreenWidth() - image.width) / 2.0f;
    position.y = GetScreenHeight() - image.height - 10;
    lastFireTime = 0.0;
    lasers.clear(); // Clear all lasers
}