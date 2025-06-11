#include <raylib.h>
#include "game.hpp"
#include <string>

std::string FormatWithLeadingZeroes(int number, int width) {
    std::string scoreText = std::to_string(number);
    int leadingZeroes = width - scoreText.length();
    return scoreText.insert(0, leadingZeroes, '0');
}

int main() {
    Color grey = { 29, 29, 27, 255 };
    Color yellow = { 243, 216, 63, 255 };
    int offset = 50;  
    int windowWidth = 750;
    int windowHeight = 700;
    InitWindow(windowWidth + offset, windowHeight + 2 * offset, "C++ Space Invaders");
    Font font = LoadFontEx("assets/fonts/monogram.ttf", 64, 0, 0);
    Texture2D spaceshipImage = LoadTexture("assets/images/spaceship.png");
    SetTargetFPS(60);

    Game game;

    while(WindowShouldClose() == false) {
        game.HandleInput();
        game.Update();
        BeginDrawing();
        ClearBackground(grey);
        DrawRectangleRoundedLines({10, 10, 780, 780}, 0.18f, 20, yellow);
        DrawLineEx({25, 730}, {775, 730}, 2, yellow);
        
        if (game.run) {
            DrawTextEx(font, "LEVEL 01", {570, 740}, 34, 2, yellow);
        } else {
            DrawTextEx(font, "GAME OVER", {570, 740}, 34, 2, yellow);
            DrawTextEx(font, "Press ENTER to restart", {200, 450}, 32, 2, yellow);
        }
        
        for(int i = 0; i < game.lives; i++) {
            DrawTexture(spaceshipImage, 50 + i * (spaceshipImage.width + 10), 740, WHITE);
        }

        DrawTextEx(font, "SCORE", {50, 15}, 34, 2, yellow);
        std::string scoreText = FormatWithLeadingZeroes(game.score, 5);
        DrawTextEx(font, scoreText.c_str(), {50, 40}, 34, 2, yellow);
        
        DrawTextEx(font, "HIGH SCORE", {550, 15}, 34, 2, yellow);
        std::string highScoreText = FormatWithLeadingZeroes(game.highScore, 5);
        DrawTextEx(font, highScoreText.c_str(), {660, 40}, 34, 2, yellow);

        game.Draw();
        EndDrawing();
    }

    CloseWindow();
}