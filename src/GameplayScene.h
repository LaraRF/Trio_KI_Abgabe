//
// Created by lrfri on 18.05.2025.
//

#ifndef RAYLIBSTARTER_GAMEPLAYSCENE_H
#define RAYLIBSTARTER_GAMEPLAYSCENE_H

#include "GameTypes.h"
#include "Scene.h"
#include "Board.h"
#include "GameState.h"
#include "GameRenderer.h"
#include "GameSolver.h"
#include "Button.h"

class GameplayScene : public Scene {
public:
    GameplayScene(int boardSize);

    void initialize() override;
    void update(float deltaTime) override;
    void draw() const override;

    SceneType getNextScene() const override;
    bool isSceneFinished() const override;

    const GameState& getGameState() const;

private:
    int boardSize;
    BoardPtr board;
    GameState gameState;
    GameRenderer renderer;
    GameSolver solver;
    std::vector<ButtonPtr> buttons;

    void generateTargetNumber();
    void processClick(int row, int col);
};

#endif //RAYLIBSTARTER_GAMEPLAYSCENE_H
