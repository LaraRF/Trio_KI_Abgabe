//
// Created by lrfri on 18.05.2025.
//

#include "GameOverScene.h"

GameOverScene::GameOverScene(const GameState& gameState) : Scene(), gameState(gameState) {}

void GameOverScene::initialize() {
    sceneFinished = false;
    nextScene = SceneType::MAIN_MENU;
}

void GameOverScene::update(float deltaTime) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        sceneFinished = true;
    }
}

void GameOverScene::draw() const {
    renderer.drawGameOverScreen(gameState);
}

SceneType GameOverScene::getNextScene() const {
    return nextScene;
}

bool GameOverScene::isSceneFinished() const {
    return sceneFinished;
}