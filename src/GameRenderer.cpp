//
// Created by lrfri on 18.05.2025.
//

#include "GameRenderer.h"
#include "Button.h"


GameRenderer::GameRenderer() {}

void GameRenderer::drawBoard(const BoardPtr& board, const GameState& gameState) const {
    if (board) {
        board->draw();
    }

    if (gameState.isRoundWon() && gameState.getRoundWonTimer() > 0.0f) {
        drawCorrectMessage();
    }
}

void GameRenderer::drawGameInfo(const GameState& gameState) const {
    // Spielinfo anzeigen
    char roundInfo[100];
    int currentRound = gameState.getRounds() + 1;
    if (currentRound > gameState.getMaxRounds()) {
        currentRound = gameState.getMaxRounds();
    }
    sprintf(roundInfo, "Runde: %d/%d", currentRound, gameState.getMaxRounds());
    DrawText(roundInfo, 10, 10, 20, BLACK);

    // Zielzahl anzeigen
    char targetInfo[100];
    sprintf(targetInfo, "Zielzahl: %d", gameState.getTargetNumber());
    DrawText(targetInfo, 10, 40, 30, RED);

    // Gesamtzeit anzeigen (laufend)
    float currentTime = gameState.getCurrentTime();
    char timeInfo[100];
    sprintf(timeInfo, "Gesamtzeit: %.1f Sekunden", currentTime);
    DrawText(timeInfo, 10, 80, 20, BLACK);

    // Durchschnittszeit anzeigen
    char avgTimeInfo[100];
    if (gameState.getPlayerScore() > 0 || gameState.getAIScore() > 0) {
        // Im Versus-Modus separate Durchschnittszeiten
        sprintf(avgTimeInfo, "Durchschnittszeiten - Spieler: %.1f s - KI: %.1f s",
                gameState.getPlayerAverageTime(), gameState.getAIAverageTime());
    } else {
        // Im Solomodus eine Gesamtdurchschnittszeit
        sprintf(avgTimeInfo, "Durchschnittszeit: %.1f Sekunden",
                gameState.getPlayerAverageTime());
    }
    DrawText(avgTimeInfo, 10, 110, 20, BLACK);

    // Falsche Versuche anzeigen
    char attemptsInfo[100];
    sprintf(attemptsInfo, "Falsche Versuche: %d", gameState.getWrongAttempts());
    DrawText(attemptsInfo, 10, 140, 20, BLACK);

    // Benutzte Hinweise anzeigen
    char hintsInfo[100];
    sprintf(hintsInfo, "Benutzte Hinweise: %d", gameState.getHintsUsed());
    DrawText(hintsInfo, 10, 170, 20, BLACK);
}

void GameRenderer::drawCorrectMessage() const {
    const char* correctText = "Richtig!";
    int fontSize = 40;
    int textWidth = MeasureText(correctText, fontSize);

    DrawText(correctText,
             (SCREEN_WIDTH - textWidth) / 2,
             SCREEN_HEIGHT / 2 - 100,
             fontSize, GREEN);
}

//void GameRenderer::drawGameOverScreen(const GameState& gameState) const {}

void GameRenderer::drawMainMenu(const std::vector<ButtonPtr>& buttons) const {
    DrawText("Wähle die Größe des Spielfelds",
             SCREEN_WIDTH/2 - MeasureText("Wähle die Größe des Spielfelds", 30)/2,
             100, 30, BLACK);

    for (const auto& button : buttons) {
        button->draw();
    }
}