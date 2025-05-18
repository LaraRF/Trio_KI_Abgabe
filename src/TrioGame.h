//
// Created by lrfri on 18.05.2025.
//

#ifndef RAYLIBSTARTER_TRIOGAME_H
#define RAYLIBSTARTER_TRIOGAME_H

#include "GameTypes.h"
#include "Scene.h"
#include "MainMenuScene.h"
#include "GameplayScene.h"
#include "GameOverScene.h"

class TrioGame {
public:
    TrioGame();
    ~TrioGame();

    void initialize();
    void run();

private:
    ScenePtr currentScene;

    void changeScene(SceneType sceneType);
};

#endif //RAYLIBSTARTER_TRIOGAME_H
