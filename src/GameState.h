//
// Created by lrfri on 18.05.2025.
//

#ifndef RAYLIBSTARTER_GAMESTATE_H
#define RAYLIBSTARTER_GAMESTATE_H

#include "GameTypes.h"
#include <vector>
#include <chrono>

class GameState {
public:
    GameState();

    void initialize();
    void update(float deltaTime);

    int getTargetNumber() const;
    void setTargetNumber(int number);

    int getRounds() const;
    int getMaxRounds() const;

    bool isGameWon() const;
    bool isRoundWon() const;

    float getRoundWonTimer() const;

    float getTotalTime() const;

    int getWrongAttempts() const;
    void incrementWrongAttempts();

    int getHintsUsed() const;
    void incrementHintsUsed();

    const std::vector<CellPosition>& getSelectedCells() const;
    void addSelectedCell(const CellPosition& cell);
    void removeSelectedCell(const CellPosition& cell);
    void clearSelectedCells();
    bool isCellSelected(const CellPosition& cell) const;

    void startNewRound();
    void winRound();

    void addUsedTargetNumber(int number);
    bool isTargetNumberUsed(int number) const;

    int getPlayerScore() const;
    int getAIScore() const;
    void incrementPlayerScore();
    void incrementAIScore();

    int maxRounds;

    float getCurrentTime() const;  // Gibt die aktuelle Gesamtzeit zurück (inkl. laufende Runde)
    float getAverageTime() const;  // Gibt die Durchschnittszeit pro gewonnener Runde zurück
    float getPlayerAverageTime() const; // Durchschnittszeit des Spielers aus allen Runden
    float getAIAverageTime() const;     // Durchschnittszeit der KI aus allen Runden

    void startRound();  // Startet eine neue Runde mit Zeitmessung
    void recordRoundTime(bool playerWon);  // Zeichnet die Zeit für die aktuelle Runde auf
    int rounds;

    void recordRoundTimeForPlayer(float time); // Zeichnet die Zeit für eine Spielerrunde auf
    void recordRoundTimeForAI(float time);     // Zeichnet die Zeit für eine KI-Runde auf

    void pauseTimer();   // Pausiert den Timer für die aktuelle Runde
    void resumeTimer();  // Setzt den Timer fort
    bool timerPaused;
    std::chrono::time_point<std::chrono::steady_clock> roundStartTime;
private:
    int targetNumber;
    std::vector<CellPosition> selectedCells;
    bool gameWon;
    bool roundWon;
    float roundWonTimer;
    float totalTime;
    int wrongAttempts;
    int hintsUsed;
    std::vector<int> usedTargetNumbers;
    int playerScore;
    int aiScore;
    std::vector<float> playerRoundTimes;  // Zeiten für alle Spielerrunden
    std::vector<float> aiRoundTimes;      // Zeiten für alle KI-Runden
    std::chrono::time_point<std::chrono::steady_clock> gameStartTime;  // Startzeit des gesamten Spiels
    std::chrono::time_point<std::chrono::steady_clock> pauseStartTime;

};

#endif //RAYLIBSTARTER_GAMESTATE_H
