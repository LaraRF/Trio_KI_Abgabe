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
    // Zeichne den halbtransparenten Hintergrund
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

    // Spielstatistiken anzeigen
    char timeInfo[100];
    sprintf(timeInfo, "Gesamtzeit: %.1f Sekunden", gameState.getCurrentTime());
    int timeWidth = MeasureText(timeInfo, 20);
    DrawText(timeInfo,
             (SCREEN_WIDTH - timeWidth) / 2,
             SCREEN_HEIGHT / 2 - 40,
             20, WHITE);

    // Durchschnittszeiten anzeigen
    int yPos = SCREEN_HEIGHT / 2 - 10;

    if (isVersusMode) {
        // Im Versus-Modus separate Durchschnittszeiten für Spieler und KI
        if (gameState.getPlayerScore() > 0) {
            char playerAvgTimeInfo[100];
            sprintf(playerAvgTimeInfo, "Durchschnittszeit Spieler: %.1f Sekunden",
                    gameState.getPlayerAverageTime());
            int playerAvgTimeWidth = MeasureText(playerAvgTimeInfo, 20);

            DrawText(playerAvgTimeInfo,
                     (SCREEN_WIDTH - playerAvgTimeWidth) / 2,
                     yPos,
                     20, WHITE);
            yPos += 30;
        }

        if (gameState.getAIScore() > 0) {
            char aiAvgTimeInfo[100];
            sprintf(aiAvgTimeInfo, "Durchschnittszeit KI: %.1f Sekunden",
                    gameState.getAIAverageTime());
            int aiAvgTimeWidth = MeasureText(aiAvgTimeInfo, 20);

            DrawText(aiAvgTimeInfo,
                     (SCREEN_WIDTH - aiAvgTimeWidth) / 2,
                     yPos,
                     20, WHITE);
            yPos += 30;
        }
    } else {
        // Im Solomodus eine Gesamtdurchschnittszeit
        char avgTimeInfo[100];
        sprintf(avgTimeInfo, "Durchschnittszeit pro Runde: %.1f Sekunden",
                gameState.getAverageTime());
        int avgTimeWidth = MeasureText(avgTimeInfo, 20);

        DrawText(avgTimeInfo,
                 (SCREEN_WIDTH - avgTimeWidth) / 2,
                 yPos,
                 20, WHITE);
        yPos += 30;
    }

    // Falsche Versuche anzeigen
    char attemptsInfo[100];
    sprintf(attemptsInfo, "Falsche Versuche: %d", gameState.getWrongAttempts());
    int attemptsWidth = MeasureText(attemptsInfo, 20);

    DrawText(attemptsInfo,
             (SCREEN_WIDTH - attemptsWidth) / 2,
             yPos,
             20, WHITE);
    yPos += 30;

    // Benutzte Hinweise anzeigen
    char hintsInfo[100];
    sprintf(hintsInfo, "Benutzte Hinweise: %d", gameState.getHintsUsed());
    int hintsWidth = MeasureText(hintsInfo, 20);

    DrawText(hintsInfo,
             (SCREEN_WIDTH - hintsWidth) / 2,
             yPos,
             20, WHITE);
    yPos += 40;

    // Klick-Anweisung
    const char* continueText = "Klicke, um zum Hauptmenü zurückzukehren";
    int continueWidth = MeasureText(continueText, 20);

    DrawText(continueText,
             (SCREEN_WIDTH - continueWidth) / 2,
             SCREEN_HEIGHT / 2 + 120,
             20, WHITE);
}

SceneType GameOverScene::getNextScene() const {
    return nextScene;
}

bool GameOverScene::isSceneFinished() const {
    return sceneFinished;
}