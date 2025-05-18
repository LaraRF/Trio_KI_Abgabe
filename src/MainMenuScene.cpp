//
// Created by lrfri on 18.05.2025.
//

#include "MainMenuScene.h"

MainMenuScene::MainMenuScene() : Scene(), selectedBoardSize(0) {
    // Initialisiere mit Standardoptionen
    gameOptions.mode = GameMode::SOLO;
    gameOptions.difficulty = AIDifficulty::MEDIUM;
    gameOptions.assistant = AssistantMode::OFF;
}

void MainMenuScene::initialize() {
    sceneFinished = false;
    nextScene = SceneType::GAMEPLAY;
    selectedBoardSize = 0;

    buttons.clear();

    // Erstelle Buttons für die Spielfeldgröße
    auto createButton5x5 = [this]() {
        selectedBoardSize = 5;
        nextScene = SceneType::GAMEPLAY;
        sceneFinished = true;
    };

    auto createButton7x7 = [this]() {
        selectedBoardSize = 7;
        nextScene = SceneType::GAMEPLAY;
        sceneFinished = true;
    };

    auto createButton10x10 = [this]() {
        selectedBoardSize = 10;
        nextScene = SceneType::GAMEPLAY;
        sceneFinished = true;
    };

    auto optionsButton = [this]() {
        nextScene = SceneType::OPTIONS;
        sceneFinished = true;
    };

    auto exitButton = [this]() {
        exit(0);  // Spiel beenden
    };

    ButtonPtr button5x5 = std::make_shared<Button>("5x5 Spielfeld", createButton5x5);
    ButtonPtr button7x7 = std::make_shared<Button>("7x7 Spielfeld", createButton7x7);
    ButtonPtr button10x10 = std::make_shared<Button>("10x10 Spielfeld", createButton10x10);
    ButtonPtr buttonOptions = std::make_shared<Button>("Optionen", optionsButton);
    ButtonPtr buttonExit = std::make_shared<Button>("Beenden", exitButton);

    button5x5->setPosition(SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 - 150);
    button5x5->setSize(300, 50);

    button7x7->setPosition(SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 - 80);
    button7x7->setSize(300, 50);

    button10x10->setPosition(SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 - 10);
    button10x10->setSize(300, 50);

    buttonOptions->setPosition(SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 60);
    buttonOptions->setSize(300, 50);

    buttonExit->setPosition(SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 130);
    buttonExit->setSize(300, 50);

    buttons.push_back(button5x5);
    buttons.push_back(button7x7);
    buttons.push_back(button10x10);
    buttons.push_back(buttonOptions);
    buttons.push_back(buttonExit);
}

void MainMenuScene::update(float deltaTime) {
    for (auto& button : buttons) {
        button->update(deltaTime);
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();

        for (auto& button : buttons) {
            button->handleMouseClick(mousePos);
        }
    }
}

void MainMenuScene::draw() const {
    renderer.drawMainMenu(buttons);
}

SceneType MainMenuScene::getNextScene() const {
    return nextScene;
}

bool MainMenuScene::isSceneFinished() const {
    return sceneFinished;
}

int MainMenuScene::getSelectedBoardSize() const {
    return selectedBoardSize;
}

// Getter und Setter für GameOptions
void MainMenuScene::setGameOptions(const GameOptions& options) {
    gameOptions = options;
}

GameOptions MainMenuScene::getGameOptions() const {
    return gameOptions;
}