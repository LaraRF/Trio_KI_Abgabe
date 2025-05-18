//
// Created by lrfri on 18.05.2025.
//

#ifndef RAYLIBSTARTER_GAMESOLVER_H
#define RAYLIBSTARTER_GAMESOLVER_H

#include "GameTypes.h"
#include "Board.h"
#include <vector>
#include <functional>

class GameSolver {
public:
    GameSolver(const BoardPtr& board);

    bool isTargetNumberSolvable(int targetNumber) const;
    std::vector<CellPosition> getHint(int targetNumber) const;
    bool generateTargetNumber(int& targetNumber, const std::vector<int>& usedNumbers) const;

private:
    BoardPtr board;

    bool dfs(int targetNumber, std::vector<CellPosition>& path) const;
    bool checkCombination(int targetNumber, const std::vector<CellPosition>& cells) const;
};

#endif //RAYLIBSTARTER_GAMESOLVER_H
