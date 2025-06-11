#include "game.hpp"
#include "laser.hpp"
#include <iostream>

Game::Game()
{
    InitGame();
}

Game::~Game()
{
    Alien::UnloadImages();
}

void Game::Update()
{
    if(run) {
        double currentTime = GetTime();
        if (currentTime - timeLastSpawn >= mysteryShipSpawnInterval) {
            if (!mysteryShip.alive) {
                mysteryShip.Spawn();
                timeLastSpawn = GetTime();
                mysteryShipSpawnInterval = GetRandomValue(10, 20);
            }
        }

        for (auto& laser : spaceship.lasers) {
            laser.Update();
        }

        MoveAliens();

        AlienFireLasers();

        for (auto& alienLaser : alienLasers) {
            alienLaser.Update();
        }

        DeleteInactiveLasers();
        // std::cout << "Number of active lasers: " << spaceship.lasers.size() << std::endl;

        mysteryShip.Update();

        CheckCollisions();
    } else {
        if (IsKeyDown(KEY_ENTER)) {
            Reset();
            InitGame();
        }
    }

    
}

void Game::Draw()
{
    spaceship.Draw();

    for (auto& laser : spaceship.lasers) {
        laser.Draw();
    }

    for (auto& obstacle : obstacles) {
        obstacle.Draw();
    }

    for (auto& alien : aliens) {
        alien.Draw();
    }

    for (auto& alienLaser : alienLasers) {
        alienLaser.Draw();
    }

    mysteryShip.Draw();
}

void Game::HandleInput()
{
    if(run) {
        if (IsKeyDown(KEY_LEFT)) {
            spaceship.MoveLeft();
        }
        else if (IsKeyDown(KEY_RIGHT)) {
            spaceship.MoveRight();
        }
        else if (IsKeyPressed(KEY_SPACE)) {
            spaceship.FireLaser();
        }
    }
}

void Game::DeleteInactiveLasers()
{
    for (auto it = spaceship.lasers.begin(); it != spaceship.lasers.end();) {
        if (!it -> active) {
            it = spaceship.lasers.erase(it); // Remove inactive laser
        } else {
            ++it; // Move to the next laser
        }
    }

    for (auto it = alienLasers.begin(); it != alienLasers.end();) {
        if (!it -> active) {
            it = alienLasers.erase(it); // Remove inactive laser
        } else {
            ++it; // Move to the next laser
        }
    }
}

std::vector<Obstacle> Game::CreateObstacles()
{
    int obstacleWidth = Obstacle::grid[0].size() * 3;
    float gap = (GetScreenWidth() - (4 * obstacleWidth)) / 5.0f;

    for (int i = 0; i < 4; ++i) {
        float offsetX = (i + 1) * gap + i * obstacleWidth;
        obstacles.push_back(Obstacle(Vector2{offsetX, float(GetScreenHeight() - 100)}));
    }
    return obstacles;
}

std::vector<Alien> Game::CreateAliens()
{
    std::vector<Alien> aliens;
    for (int row = 0; row < 5; ++row) {
        for (int col = 0; col < 11; ++col) {
            int type;
            if (row == 0) {
                type = 3;
            } else if (row == 1 || row == 2) {
                type = 2;
            } else {
                type = 1;
            }
            Vector2 position = { float(col * 55 + 75), float(row * 55 + 110) };
            aliens.push_back(Alien(type, position));
        }
    }
    return aliens;
}

void Game::MoveAliens()
{
    for (auto& alien : aliens) {
        if (alien.position.x + alien.alienImages[alien.type - 1].width >= GetScreenWidth())
        {
            aliensDirection = -1;
            MoveAliensDown(4);
        }
        if (alien.position.x <= 0)
        {
            aliensDirection = 1;
            MoveAliensDown(4);
        }
        alien.Update(aliensDirection);
    }
}

void Game::MoveAliensDown(int distance)
{
    for (auto& alien : aliens) {
        alien.position.y += distance;
    }
}

void Game::AlienFireLasers()
{
    double currentTime = GetTime();
    if (currentTime - lastAlienFireTime >= alienLaserFireInterval && !aliens.empty()) {
        int randomIndex = GetRandomValue(0, aliens.size() - 1);
        Alien& alien = aliens[randomIndex];
        alienLasers.push_back(Laser(Vector2{
            alien.position.x + alien.alienImages[alien.type - 1].width / 2.0f - 2.5f, 
            alien.position.y + alien.alienImages[alien.type - 1].height
        }, 6.0f));

        lastAlienFireTime = GetTime();
    }
    
}

void Game::CheckCollisions()
{
    // Spaceship lasers
    for (auto& laser: spaceship.lasers) {
        auto it = aliens.begin();
        while(it != aliens.end()) {
            if(CheckCollisionRecs(it -> getRect(), laser.getRect()))
            {
                it = aliens.erase(it); // Remove alien if hit by laser
                laser.active = false; // Deactivate the laser
            } else {
                ++it; // Move to the next alien
            }
        }

        for(auto& obstacle : obstacles) {
            auto it = obstacle.blocks.begin();
            while(it != obstacle.blocks.end()) {
                if(CheckCollisionRecs(it -> getRect(), laser.getRect())) {
                    it = obstacle.blocks.erase(it); // Remove block if hit by laser
                    laser.active = false; // Deactivate the laser
                } else {
                    ++it; // Move to the next block
                }
            }
        }

        if (CheckCollisionRecs(mysteryShip.getRect(), laser.getRect())) {
            mysteryShip.alive = false; // Deactivate mystery ship if hit by laser
            laser.active = false; // Deactivate the laser
        }
    }

    // Alien lasers
    for(auto& alienLasers : alienLasers) {
        if(CheckCollisionRecs(spaceship.getRect(), alienLasers.getRect())) {
            // If the spaceship is hit by an alien laser
            alienLasers.active = false; // Deactivate the alien laser
            lives--; // Decrease lives
            if (lives <= 0) {
                GameOver(); // Handle game over logic
            }
            
        }

        for(auto& obstacle : obstacles) {
            auto it = obstacle.blocks.begin();
            while(it != obstacle.blocks.end()) {
                if(CheckCollisionRecs(it -> getRect(), alienLasers.getRect())) {
                    it = obstacle.blocks.erase(it); // Remove block if hit by alien laser
                    alienLasers.active = false; // Deactivate the alien laser
                } else {
                    ++it; // Move to the next block
                }
            }
        }
    }

    // Alien collisions with obstacles
    for (auto& alien: aliens) {
        for (auto& obstacle: obstacles) {
            auto it = obstacle.blocks.begin();
            while(it != obstacle.blocks.end()) {
                if(CheckCollisionRecs(it -> getRect(), alien.getRect())) {
                    it = obstacle.blocks.erase(it); // Remove block if hit by alien
                } else {
                    it++; // Move to the next block
                }
            }
        }

        if(CheckCollisionRecs(spaceship.getRect(), alien.getRect())) {
            // If the spaceship is hit by an alien
            GameOver(); // Handle game over logic
        }
    }
}

void Game::GameOver()
{
    run = false; // Stop the game loop
}

void Game::InitGame()
{
    obstacles = CreateObstacles();
    aliens = CreateAliens();
    aliensDirection = 1;
    lastAlienFireTime = 0.0f;
    mysteryShip.Spawn();
    timeLastSpawn = 0.0f;
    mysteryShipSpawnInterval = GetRandomValue(10, 20);
    lives = 3; // Initialize lives
    run = true;
}

void Game::Reset()
{
    spaceship.Reset();
    aliens.clear();
    alienLasers.clear();
    obstacles.clear();
}
