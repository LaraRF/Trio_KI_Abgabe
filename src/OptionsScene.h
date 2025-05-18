//
// Created by lrfri on 18.05.2025.
//

#ifndef RAYLIBSTARTER_OPTIONSSCENE_H
#define RAYLIBSTARTER_OPTIONSSCENE_H

#include "GameTypes.h"
#include "Scene.h"
#include "Button.h"
#include <vector>

class OptionsScene : public Scene {
public:
    OptionsScene(const GameOptions& currentOptions);

    void initialize() override;
    void update(float deltaTime) override;
    void draw() const override;

    SceneType getNextScene() const override;
    bool isSceneFinished() const override;

    GameOptions getOptions() const;

private:
    GameOptions options;
    std::vector<ButtonPtr> buttons;
    void updateSelectedButtons();

    void createButtons();
};

#endif //RAYLIBSTARTER_OPTIONSSCENE_H
