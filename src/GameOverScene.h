//
// Created by lrfri on 18.05.2025.
//

#ifndef RAYLIBSTARTER_GAMEOVERSCENE_H
#define RAYLIBSTARTER_GAMEOVERSCENE_H

#include "GameTypes.h"
#include "Scene.h"
#include "GameState.h"
#include "GameRenderer.h"

class GameOverScene : public Scene {
public:
    GameOverScene(const GameState& gameState);

    void initialize() override;
    void update(float deltaTime) override;
    void draw() const override;

    SceneType getNextScene() const override;
    bool isSceneFinished() const override;

private:
    GameState gameState;
    GameRenderer renderer;
};

#endif //RAYLIBSTARTER_GAMEOVERSCENE_H
