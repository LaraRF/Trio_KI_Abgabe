//
// Created by lrfri on 18.05.2025.
//

#include "GameOverScene.h"

GameOverScene::GameOverScene(const GameState& gameState) : Scene(), gameState(gameState) {}

void GameOverScene::initialize() {
    sceneFinished = false;
    nextScene = SceneType::MAIN_MENU;
}

void GameOverScene::update(float deltaTime) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        sceneFinished = true;
    }
}

void GameOverScene::draw() const {

    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ColorAlpha(BLACK, 0.7f));

    // Bestimme den Gewinner (nur im Versus-Modus)
    bool isVersusMode = gameState.getPlayerScore() > 0 || gameState.getAIScore() > 0;
    bool playerWon = gameState.getPlayerScore() > gameState.getAIScore();

    // Titel je nach Spielmodus anpassen
    const char* gameOverText;
    if (isVersusMode) {
        if (playerWon) {
            gameOverText = "Glückwunsch - Du hast gewonnen!";
        } else if (gameState.getPlayerScore() == gameState.getAIScore()) {
            gameOverText = "Unentschieden!";
        } else {
            gameOverText = "Schade - Die KI hat gewonnen!";
        }
    } else {
        gameOverText = "Glückwunsch - Du hast alle Runden geschafft!";
    }

    int textWidth = MeasureText(gameOverText, 40);
    DrawText(gameOverText,
             (SCREEN_WIDTH - textWidth) / 2,
             SCREEN_HEIGHT / 2 - 150,
             40, WHITE);

    // Im Versus-Modus den Punktestand anzeigen
    if (isVersusMode) {
        char scoreText[100];
        sprintf(scoreText, "Endstand - Spieler: %d  KI: %d",
                gameState.getPlayerScore(), gameState.getAIScore());

        int scoreWidth = MeasureText(scoreText, 30);
        DrawText(scoreText,
                 (SCREEN_WIDTH - scoreWidth) / 2,
                 SCREEN_HEIGHT / 2 - 90,
                 30, WHITE);
    }

    renderer.drawGameOverScreen(gameState);

    const char* continueText = "Klicke, um zum Hauptmenü zurückzukehren";
    int continueWidth = MeasureText(continueText, 20);

    DrawText(continueText,
             (SCREEN_WIDTH - continueWidth) / 2,
             SCREEN_HEIGHT / 2 + 90,
             20, WHITE);
}

SceneType GameOverScene::getNextScene() const {
    return nextScene;
}

bool GameOverScene::isSceneFinished() const {
    return sceneFinished;
}