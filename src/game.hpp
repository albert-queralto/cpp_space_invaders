#pragma once
#include "spaceship.hpp"
#include "obstacle.hpp"
#include "alien.hpp"

class Game {
    public:
        Game();
        ~Game();
        void Draw();
        void Update();
        void HandleInput();

    private:
        Spaceship spaceship;
        void DeleteInactiveLasers();
        std::vector<Obstacle> obstacles;
        std::vector<Obstacle> CreateObstacles();
        std::vector<Alien> aliens;
        std::vector<Alien> CreateAliens();
        void MoveAliens();
        void MoveAliensDown(int distance);
        int aliensDirection;
        std::vector<Laser> alienLasers;
        void AlienFireLasers();
        constexpr static float alienLaserFireInterval = 0.35;
        float lastAlienFireTime;
};