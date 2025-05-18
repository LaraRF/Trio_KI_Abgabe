//
// Created by lrfri on 18.05.2025.
//

#ifndef RAYLIBSTARTER_SCENE_H
#define RAYLIBSTARTER_SCENE_H

#include "GameTypes.h"

class Scene {
public:
    Scene();
    virtual ~Scene() = default;

    virtual void initialize() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void draw() const = 0;

    virtual SceneType getNextScene() const = 0;
    virtual bool isSceneFinished() const = 0;

protected:
    SceneType nextScene;
    bool sceneFinished;
};


#endif //RAYLIBSTARTER_SCENE_H
