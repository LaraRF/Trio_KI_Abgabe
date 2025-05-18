//
// Created by lrfri on 18.05.2025.
//

#ifndef RAYLIBSTARTER_MAINMENUSCENE_H
#define RAYLIBSTARTER_MAINMENUSCENE_H

#include "GameTypes.h"
#include "Scene.h"
#include "GameRenderer.h"
#include "Button.h"

class MainMenuScene : public Scene {
public:
    MainMenuScene();

    void initialize() override;
    void update(float deltaTime) override;
    void draw() const override;

    SceneType getNextScene() const override;
    bool isSceneFinished() const override;

    int getSelectedBoardSize() const;

    void setGameOptions(const GameOptions& options);
    GameOptions getGameOptions() const;

private:
    std::vector<ButtonPtr> buttons;
    GameRenderer renderer;
    int selectedBoardSize;
    GameOptions gameOptions;
};
#endif //RAYLIBSTARTER_MAINMENUSCENE_H
