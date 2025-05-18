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

    // Zeit anzeigen
    float currentTime = gameState.getTotalTime();

    char timeInfo[100];
    sprintf(timeInfo, "Zeit: %.1f Sekunden", currentTime);
    DrawText(timeInfo, 10, 80, 20, BLACK);

    // Durchschnittszeit pro Runde anzeigen
    char avgTimeInfo[100];
    float avgTime = 0;
    if (gameState.getRounds() > 0) {
        avgTime = currentTime / gameState.getRounds();
    }
    sprintf(avgTimeInfo, "Durchschnittszeit: %.1f Sekunden", avgTime);
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

void GameRenderer::drawGameOverScreen(const GameState& gameState) const {
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ColorAlpha(BLACK, 0.7f));

    const char* gameOverText = "TrioGame Over - Du hast gewonnen!";
    int textWidth = MeasureText(gameOverText, 40);

    DrawText(gameOverText,
             (SCREEN_WIDTH - textWidth) / 2,
             SCREEN_HEIGHT / 2 - 100,
             40, WHITE);

    // Statistik anzeigen
    char timeInfo[100];
    sprintf(timeInfo, "Gesamtzeit: %.1f Sekunden", gameState.getTotalTime());
    int timeWidth = MeasureText(timeInfo, 20);
    DrawText(timeInfo,
             (SCREEN_WIDTH - timeWidth) / 2,
             SCREEN_HEIGHT / 2 - 40,
             20, WHITE);

    // Durchschnittszeit pro Runde anzeigen
    char avgTimeInfo[100];
    float avgTime = gameState.getMaxRounds() > 0 ? gameState.getTotalTime() / gameState.getMaxRounds() : 0;
    sprintf(avgTimeInfo, "Durchschnittszeit: %.1f Sekunden", avgTime);
    int avgTimeWidth = MeasureText(avgTimeInfo, 20);
    DrawText(avgTimeInfo,
             (SCREEN_WIDTH - avgTimeWidth) / 2,
             SCREEN_HEIGHT / 2 - 10,
             20, WHITE);

    char attemptsInfo[100];
    sprintf(attemptsInfo, "Falsche Versuche: %d", gameState.getWrongAttempts());
    int attemptsWidth = MeasureText(attemptsInfo, 20);
    DrawText(attemptsInfo,
             (SCREEN_WIDTH - attemptsWidth) / 2,
             SCREEN_HEIGHT / 2 + 20,
             20, WHITE);

    char hintsInfo[100];
    sprintf(hintsInfo, "Benutzte Hinweise: %d", gameState.getHintsUsed());
    int hintsWidth = MeasureText(hintsInfo, 20);
    DrawText(hintsInfo,
             (SCREEN_WIDTH - hintsWidth) / 2,
             SCREEN_HEIGHT / 2 + 50,
             20, WHITE);

    const char* continueText = "Klicke, um zum Hauptmenü zurückzukehren";
    int continueWidth = MeasureText(continueText, 20);

    DrawText(continueText,
             (SCREEN_WIDTH - continueWidth) / 2,
             SCREEN_HEIGHT / 2 + 90,
             20, WHITE);
}

void GameRenderer::drawMainMenu(const std::vector<ButtonPtr>& buttons) const {
    DrawText("Wähle die Größe des Spielfelds",
             SCREEN_WIDTH/2 - MeasureText("Wähle die Größe des Spielfelds", 30)/2,
             100, 30, BLACK);

    for (const auto& button : buttons) {
        button->draw();
    }
}