//
// Created by lrfri on 18.05.2025.
//

#ifndef RAYLIBSTARTER_GAMERENDERER_H
#define RAYLIBSTARTER_GAMERENDERER_H

#include "GameTypes.h"
#include "Board.h"
#include "GameState.h"

class GameRenderer {
public:
    GameRenderer();

    void drawBoard(const BoardPtr& board, const GameState& gameState) const;
    void drawGameInfo(const GameState& gameState) const;
    void drawGameOverScreen(const GameState& gameState) const;
    void drawMainMenu(const std::vector<ButtonPtr>& buttons) const;

private:
    void drawCorrectMessage() const;
};

#endif //RAYLIBSTARTER_GAMERENDERER_H
