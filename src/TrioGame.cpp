//
// Created by lrfri on 18.05.2025.
//

#include "TrioGame.h"

TrioGame::TrioGame() : currentScene(nullptr) {}

TrioGame::~TrioGame() {
}

void TrioGame::initialize() {

    // Starte mit dem Hauptmenü
    changeScene(SceneType::MAIN_MENU);
}

void TrioGame::run() {
    while (!WindowShouldClose()) {
        // Delta-Zeit für die Animation
        float deltaTime = GetFrameTime();

        // Update aktuelle Szene
        if (currentScene) {
            currentScene->update(deltaTime);

            // Prüfe, ob die Szene beendet ist und wechsle zur nächsten
            if (currentScene->isSceneFinished()) {
                changeScene(currentScene->getNextScene());
            }
        }

        // Zeichnen
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (currentScene) {
            currentScene->draw();
        }

        EndDrawing();
    }
}

void TrioGame::changeScene(SceneType sceneType) {
    switch (sceneType) {
        case SceneType::MAIN_MENU:
            currentScene = std::make_shared<MainMenuScene>();
            break;

        case SceneType::GAMEPLAY:
        {
            auto mainMenuScene = std::dynamic_pointer_cast<MainMenuScene>(currentScene);
            if (mainMenuScene) {
                int boardSize = mainMenuScene->getSelectedBoardSize();
                currentScene = std::make_shared<GameplayScene>(boardSize);
            } else {
                // Fallback: Verwende Standardgröße
                currentScene = std::make_shared<GameplayScene>(7);
            }
        }
            break;

        case SceneType::GAME_OVER:
        {
            auto gameplayScene = std::dynamic_pointer_cast<GameplayScene>(currentScene);
            if (gameplayScene) {
                currentScene = std::make_shared<GameOverScene>(gameplayScene->getGameState());
            } else {
                // Fallback: Erstelle TrioGame-Over-Szene mit leerem GameState
                GameState emptyState;
                currentScene = std::make_shared<GameOverScene>(emptyState);
            }
        }
            break;
    }

    if (currentScene) {
        currentScene->initialize();
    }
}