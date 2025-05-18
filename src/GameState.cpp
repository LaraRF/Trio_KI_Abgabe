#include "GameState.h"
#include <algorithm>

GameState::GameState() {
    initialize();
}

void GameState::initialize() {
    targetNumber = 0;
    rounds = 0;
    maxRounds = 10;
    selectedCells.clear();
    gameWon = false;
    roundWon = false;
    roundWonTimer = 0.0f;
    totalTime = 0.0f;
    wrongAttempts = 0;
    hintsUsed = 0;
    usedTargetNumbers.clear();
    roundStartTime = std::chrono::steady_clock::now();
    playerScore = 0;
    aiScore = 0;
    playerRoundTimes.clear();
    aiRoundTimes.clear();
    gameStartTime = std::chrono::steady_clock::now();
    roundStartTime = gameStartTime;
    timerPaused = false;
}

void GameState::update(float deltaTime) {
    if (roundWon) {
        roundWonTimer -= deltaTime;

        if (roundWonTimer <= 0.0f) {
            roundWonTimer = 0.0f;
        }
    }
}

int GameState::getTargetNumber() const {
    return targetNumber;
}

void GameState::setTargetNumber(int number) {
    targetNumber = number;
}

int GameState::getRounds() const {
    return rounds;
}

int GameState::getMaxRounds() const {
    return maxRounds;
}

bool GameState::isGameWon() const {
    return gameWon;
}

bool GameState::isRoundWon() const {
    return roundWon;
}

float GameState::getRoundWonTimer() const {
    return roundWonTimer;
}

float GameState::getTotalTime() const {
    return totalTime;
}

int GameState::getWrongAttempts() const {
    return wrongAttempts;
}

void GameState::incrementWrongAttempts() {
    wrongAttempts++;
}

int GameState::getHintsUsed() const {
    return hintsUsed;
}

void GameState::incrementHintsUsed() {
    hintsUsed++;
}

const std::vector<CellPosition>& GameState::getSelectedCells() const {
    return selectedCells;
}

void GameState::addSelectedCell(const CellPosition& cell) {
    selectedCells.push_back(cell);
}

void GameState::removeSelectedCell(const CellPosition& cell) {
    selectedCells.erase(
            std::remove_if(selectedCells.begin(), selectedCells.end(),
                           [&cell](const CellPosition& pos) {
                               return pos.row == cell.row && pos.col == cell.col;
                           }),
            selectedCells.end());
}

void GameState::clearSelectedCells() {
    selectedCells.clear();
}

bool GameState::isCellSelected(const CellPosition& cell) const {
    return std::find_if(selectedCells.begin(), selectedCells.end(),
                        [&cell](const CellPosition& pos) {
                            return pos.row == cell.row && pos.col == cell.col;
                        }) != selectedCells.end();
}

void GameState::startNewRound() {
    auto now = std::chrono::steady_clock::now();
    if (!roundWon) {
        // Aktualisiere die Zeit nur, wenn die Runde nicht bereits gewonnen ist
        std::chrono::duration<float> elapsed = now - roundStartTime;
        totalTime += elapsed.count();
    }

    selectedCells.clear();
    roundWon = false;
    roundStartTime = now;
}

void GameState::winRound() {
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsed;

    if (timerPaused) {
        // Wenn der Timer pausiert ist, berechne die Zeit bis zur Pause
        elapsed = pauseStartTime - roundStartTime;
    } else {
        elapsed = now - roundStartTime;
    }

    totalTime += elapsed.count();

    roundWon = true;
    roundWonTimer = 2.0f; // 2 Sekunden anzeigen

    // Pausiere den Timer während "Richtig!" angezeigt wird
    pauseTimer();

    // Überprüfe, ob wir die maximale Rundenanzahl erreicht haben
    if (rounds + 1 >= maxRounds) {
        gameWon = true;
    }
}

void GameState::addUsedTargetNumber(int number) {
    usedTargetNumbers.push_back(number);
}

bool GameState::isTargetNumberUsed(int number) const {
    return std::find(usedTargetNumbers.begin(), usedTargetNumbers.end(), number) != usedTargetNumbers.end();
}

int GameState::getPlayerScore() const {
    return playerScore;
}

int GameState::getAIScore() const {
    return aiScore;
}

void GameState::incrementPlayerScore() {
    playerScore++;
}

void GameState::incrementAIScore() {
    aiScore++;
}

float GameState::getCurrentTime() const {
    if (gameWon) {
        // Wenn das Spiel gewonnen ist, gib die endgültige Gesamtzeit zurück
        return totalTime;
    }

    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<float> totalElapsed = now - gameStartTime;

    if (timerPaused) {
        // Wenn der Timer pausiert ist, ziehe die Pausenzeit ab
        std::chrono::duration<float> pauseTime = now - pauseStartTime;
        return totalElapsed.count() - pauseTime.count();
    }

    return totalElapsed.count();
}

float GameState::getAverageTime() const {
    if (playerRoundTimes.empty() && aiRoundTimes.empty()) {
        return 0.0f;
    }

    float totalTime = 0.0f;
    for (float time : playerRoundTimes) {
        totalTime += time;
    }
    for (float time : aiRoundTimes) {
        totalTime += time;
    }

    return totalTime / (playerRoundTimes.size() + aiRoundTimes.size());
}

float GameState::getPlayerAverageTime() const {
    if (playerRoundTimes.empty()) {
        return 0.0f;
    }

    float totalTime = 0.0f;
    for (float time : playerRoundTimes) {
        totalTime += time;
    }

    return totalTime / playerRoundTimes.size();
}

float GameState::getAIAverageTime() const {
    if (aiRoundTimes.empty()) {
        return 0.0f;
    }

    float totalTime = 0.0f;
    for (float time : aiRoundTimes) {
        totalTime += time;
    }

    return totalTime / aiRoundTimes.size();
}

void GameState::startRound() {
    roundStartTime = std::chrono::steady_clock::now();
    selectedCells.clear();
    roundWon = false;
}

void GameState::recordRoundTime(bool playerWon) {
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsed = now - roundStartTime;
    float roundTime = elapsed.count();

    if (playerWon) {
        playerRoundTimes.push_back(roundTime);
    } else {
        aiRoundTimes.push_back(roundTime);
    }
}

void GameState::pauseTimer() {
    if (!timerPaused) {
        timerPaused = true;
        pauseStartTime = std::chrono::steady_clock::now();
    }
}

void GameState::resumeTimer() {
    if (timerPaused) {
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<long long int, std::ratio<1, 1000000000>> pauseDuration = now - pauseStartTime;

        // Verschiebe den Rundenstartzeitpunkt um die Pausendauer nach hinten
        roundStartTime += pauseDuration;

        timerPaused = false;
    }
}
