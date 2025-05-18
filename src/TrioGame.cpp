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
    GameOptions currentOptions;
    int selectedBoardSize = 7; // Standardgröße

    // Speichere aktuelle Werte und Optionen, falls vorhanden
    auto mainMenuScene = std::dynamic_pointer_cast<MainMenuScene>(currentScene);
    if (mainMenuScene) {
        currentOptions = mainMenuScene->getGameOptions();
        selectedBoardSize = mainMenuScene->getSelectedBoardSize();
    }

    auto optionsScene = std::dynamic_pointer_cast<OptionsScene>(currentScene);
    if (optionsScene) {
        currentOptions = optionsScene->getOptions();
    }

    // Erstelle neue Szene basierend auf dem gewünschten Typ
    switch (sceneType) {
        case SceneType::MAIN_MENU:
        {
            auto newMainMenu = std::make_shared<MainMenuScene>();

            // Übertrage die aktuellen Optionen vom Optionsmenü
            if (optionsScene) {
                newMainMenu->setGameOptions(currentOptions);
            }
                // Oder behalte die vorherigen Optionen bei
            else if (mainMenuScene) {
                newMainMenu->setGameOptions(currentOptions);
            }

            currentScene = newMainMenu;
        }
            break;

        case SceneType::OPTIONS:
            currentScene = std::make_shared<OptionsScene>(currentOptions);
            break;

        case SceneType::GAMEPLAY:
        {
            // Wenn wir vom Hauptmenü kommen, hole die Spielfeldgröße und Optionen von dort
            if (mainMenuScene) {
                selectedBoardSize = mainMenuScene->getSelectedBoardSize();
                currentOptions = mainMenuScene->getGameOptions();
            }

            currentScene = std::make_shared<GameplayScene>(selectedBoardSize, currentOptions);
        }
            break;

        case SceneType::GAME_OVER:
        {
            auto gameplayScene = std::dynamic_pointer_cast<GameplayScene>(currentScene);
            if (gameplayScene) {
                currentScene = std::make_shared<GameOverScene>(gameplayScene->getGameState());
            } else {
                // Fallback: Erstelle Game-Over-Szene mit leerem GameState
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