//
// Created by lrfri on 18.05.2025.
//

#include "GameSolver.h"
#include <random>

GameSolver::GameSolver(const BoardPtr& board) : board(board) {}

bool GameSolver::isTargetNumberSolvable(int targetNumber) const {
    std::vector<CellPosition> path;
    return dfs(targetNumber, path);
}

std::vector<CellPosition> GameSolver::getHint(int targetNumber) const {
    std::vector<CellPosition> path;
    dfs(targetNumber, path);
    return path;
}

bool GameSolver::generateTargetNumber(int& targetNumber, const std::vector<int>& usedNumbers) const {
    std::random_device rd;
    std::mt19937 gen(rd());

    // Generiere eine lösbare Zielzahl, die noch nicht verwendet wurde
    bool isSolvable = false;
    bool isUnique = false;

    // Suche nach einer lösbaren und eindeutigen Zielzahl
    int attempts = 0;
    const int maxAttempts = 100; // Maximale Anzahl an Versuchen

    while ((!isSolvable || !isUnique) && attempts < maxAttempts) {
        std::uniform_int_distribution<> distrib(1, 50);
        targetNumber = distrib(gen);

        // Prüfe, ob die Zahl bereits verwendet wurde
        isUnique = std::find(usedNumbers.begin(), usedNumbers.end(), targetNumber) == usedNumbers.end();

        // Prüfe, ob die Zahl lösbar ist
        if (isUnique) {
            isSolvable = isTargetNumberSolvable(targetNumber);
        }

        attempts++;
    }

    // Wenn keine lösbare und eindeutige Zahl gefunden wurde, verwende eine, die zumindest lösbar ist
    if (!isSolvable || !isUnique) {
        attempts = 0;
        while (!isSolvable && attempts < maxAttempts) {
            std::uniform_int_distribution<> distrib(1, 50);
            targetNumber = distrib(gen);
            isSolvable = isTargetNumberSolvable(targetNumber);
            attempts++;
        }

        // Wenn immer noch keine lösbare Zahl gefunden wurde, verwende eine Standardzahl
        if (!isSolvable) {
            targetNumber = 12; // Eine einfache Zahl, die oft lösbar ist
            return false;
        }
    }

    return true;
}

bool GameSolver::dfs(int targetNumber, std::vector<CellPosition>& path) const {
    // Wenn wir bereits 3 Zellen haben, prüfe ob sie die Zielzahl ergeben
    if (path.size() == 3) {
        return checkCombination(targetNumber, path);
    }

    // Wenn wir noch keine Zellen haben, starte an jeder möglichen Position
    if (path.empty()) {
        for (int row = 0; row < board->getSize(); row++) {
            for (int col = 0; col < board->getSize(); col++) {
                path.push_back({row, col});
                if (dfs(targetNumber, path)) {
                    return true;
                }
                path.pop_back();
            }
        }
    } else {
        // Andernfalls erweitere den aktuellen Pfad
        std::vector<CellPosition> neighbors = board->getValidNextCells(path);

        for (const auto& neighbor : neighbors) {
            path.push_back(neighbor);
            if (dfs(targetNumber, path)) {
                return true;
            }
            path.pop_back();
        }
    }

    return false;
}

bool GameSolver::checkCombination(int targetNumber, const std::vector<CellPosition>& cells) const {
    // Prüfe, ob die Zellen eine gültige Reihe bilden
    if (!board->isValidSelection(cells)) {
        return false;
    }

    // Prüfe, ob die Kombination die Zielzahl ergibt
    return board->checkWinCondition(cells, targetNumber);
}