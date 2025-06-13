#pragma once
#include "spaceship.hpp"
#include "obstacle.hpp"
#include "alien.hpp"
#include "mysteryship.hpp"

class Game {
    public:
        Game();
        ~Game();
        void Draw();
        void Update();
        void HandleInput();
        bool run;
        int lives;
        int score;
        int highScore;
        Music music;

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
        MysteryShip mysteryShip;
        float mysteryShipSpawnInterval;
        float timeLastSpawn;
        void CheckCollisions();
        void GameOver();
        void Reset();
        void InitGame();
        void CheckHighScore();
        void SaveHighScoreToFile(int highscore);
        int LoadHighScoreFromFile();
        Sound explosionSound;
};