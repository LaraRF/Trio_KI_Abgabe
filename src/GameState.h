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
private:
    int targetNumber;
    int rounds;
    std::vector<CellPosition> selectedCells;
    bool gameWon;
    bool roundWon;
    float roundWonTimer;
    std::chrono::time_point<std::chrono::steady_clock> roundStartTime;
    float totalTime;
    int wrongAttempts;
    int hintsUsed;
    std::vector<int> usedTargetNumbers;
    int playerScore;
    int aiScore;
};

#endif //RAYLIBSTARTER_GAMESTATE_H
