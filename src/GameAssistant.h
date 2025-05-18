//
// Created by lrfri on 18.05.2025.
//

#ifndef RAYLIBSTARTER_GAMEASSISTANT_H
#define RAYLIBSTARTER_GAMEASSISTANT_H

#include "GameTypes.h"
#include "Board.h"
#include <vector>
#include <string>

class GameAssistant {
public:
    GameAssistant();

    // Generiere einen Hinweis basierend auf dem aktuellen Spielzustand
    std::string generateHint(const BoardPtr& board, int targetNumber, const std::vector<CellPosition>& selectedCells, int hintLevel);

private:
    // Vorbereitete Hinweistexte für verschiedene Situationen
    std::vector<std::string> basicHints;
    std::vector<std::string> advancedHints;

    // Analysiere das Spielfeld und generiere spezifische Hinweise
    std::string analyzeGameState(const BoardPtr& board, int targetNumber, const std::vector<CellPosition>& selectedCells);

    // Hilfsfunktionen für die Analyse
    std::vector<CellPosition> findPartialSolution(const BoardPtr& board, int targetNumber, const std::vector<CellPosition>& selectedCells);
    std::string describeLocation(const CellPosition& position, const BoardPtr& board);
};

#endif //RAYLIBSTARTER_GAMEASSISTANT_H
