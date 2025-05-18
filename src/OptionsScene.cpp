//
// Created by lrfri on 18.05.2025.
//

#include "OptionsScene.h"
#include <string>

OptionsScene::OptionsScene(const GameOptions& currentOptions)
        : Scene(), options(currentOptions) {
}

void OptionsScene::initialize() {
    sceneFinished = false;
    nextScene = SceneType::MAIN_MENU;

    createButtons();
}

void OptionsScene::update(float deltaTime) {
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

void OptionsScene::draw() const {
    // Zeichne den Hintergrund
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RAYWHITE);

    // Zeichne den Titel
    DrawText("Spieloptionen", SCREEN_WIDTH/2 - MeasureText("Spieloptionen", 40)/2, 50, 40, BLACK);

    // Zeichne die Optionskategorien
    DrawText("Spielmodus:", 100, 150, 30, BLACK);
    DrawText("Schwierigkeitsgrad:", 100, 250, 30, BLACK);
    DrawText("Assistent:", 100, 350, 30, BLACK);

    // Zeige die aktuell ausgewählten Optionen
    Color highlightColor = DARKBLUE;

    // Spielmodus
    const char* modeText = (options.mode == GameMode::SOLO) ? "Solo" : "Gegen KI";
    DrawText(TextFormat("Aktiv: %s", modeText), 100, 190, 20, highlightColor);

    // Schwierigkeitsgrad
    const char* difficultyText;
    switch (options.difficulty) {
        case AIDifficulty::EASY: difficultyText = "Einfach"; break;
        case AIDifficulty::MEDIUM: difficultyText = "Mittel"; break;
        case AIDifficulty::HARD: difficultyText = "Schwer"; break;
        default: difficultyText = "Unbekannt";
    }
    DrawText(TextFormat("Aktiv: %s", difficultyText), 100, 290, 20, highlightColor);

    // Assistent
    const char* assistantText = (options.assistant == AssistantMode::ON) ? "An" : "Aus";
    DrawText(TextFormat("Aktiv: %s", assistantText), 100, 390, 20, highlightColor);


    // Zeichne alle Buttons
    for (const auto& button : buttons) {
        button->draw();
    }

    // Zurück-Button
    DrawText("Zum Speichern und Zurückkehren auf 'Zurück' klicken",
             SCREEN_WIDTH/2 - MeasureText("Zum Speichern und Zurückkehren auf 'Zurück' klicken", 20)/2,
             SCREEN_HEIGHT - 70, 20, DARKGRAY);
}

SceneType OptionsScene::getNextScene() const {
    return nextScene;
}

bool OptionsScene::isSceneFinished() const {
    return sceneFinished;
}

GameOptions OptionsScene::getOptions() const {
    return options;
}

void OptionsScene::createButtons() {
    buttons.clear();

    // Erstelle Buttons für Spielmodus
    auto soloButton = std::make_shared<Button>("Spiele alleine", [this]() {
        options.mode = GameMode::SOLO;
        updateSelectedButtons();
    });
    soloButton->setPosition(400, 150);
    soloButton->setSize(200, 40);

    auto vsAiButton = std::make_shared<Button>("Spiele gegen KI", [this]() {
        options.mode = GameMode::VERSUS_AI;
        updateSelectedButtons();
    });
    vsAiButton->setPosition(610, 150);
    vsAiButton->setSize(200, 40);

    // Erstelle Buttons für Schwierigkeitsgrad
    auto easyButton = std::make_shared<Button>("Einfach", [this]() {
        options.difficulty = AIDifficulty::EASY;
        updateSelectedButtons();
    });
    easyButton->setPosition(400, 250);
    easyButton->setSize(120, 40);

    auto mediumButton = std::make_shared<Button>("Mittel", [this]() {
        options.difficulty = AIDifficulty::MEDIUM;
        updateSelectedButtons();
    });
    mediumButton->setPosition(530, 250);
    mediumButton->setSize(120, 40);

    auto hardButton = std::make_shared<Button>("Schwer", [this]() {
        options.difficulty = AIDifficulty::HARD;
        updateSelectedButtons();
    });
    hardButton->setPosition(660, 250);
    hardButton->setSize(120, 40);

    // Erstelle Buttons für Assistent
    auto assistantOffButton = std::make_shared<Button>("Aus", [this]() {
        options.assistant = AssistantMode::OFF;
        updateSelectedButtons();
    });
    assistantOffButton->setPosition(400, 350);
    assistantOffButton->setSize(150, 40);

    auto assistantOnButton = std::make_shared<Button>("An", [this]() {
        options.assistant = AssistantMode::ON;
        updateSelectedButtons();
    });
    assistantOnButton->setPosition(560, 350);
    assistantOnButton->setSize(150, 40);

    // Erstelle Zurück-Button
    auto backButton = std::make_shared<Button>("Zurück", [this]() {
        // Stelle sicher, dass die Optionen gespeichert werden
        sceneFinished = true;
        nextScene = SceneType::MAIN_MENU;
    });
    backButton->setPosition(SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT - 120);
    backButton->setSize(200, 50);

    // Füge alle Buttons hinzu
    buttons.push_back(soloButton);
    buttons.push_back(vsAiButton);
    buttons.push_back(easyButton);
    buttons.push_back(mediumButton);
    buttons.push_back(hardButton);
    buttons.push_back(assistantOffButton);
    buttons.push_back(assistantOnButton);
    buttons.push_back(backButton);

    // Markiere anfangs die ausgewählten Buttons
    updateSelectedButtons();
}

void OptionsScene::updateSelectedButtons() {
    for (auto& button : buttons) {
        // Setze alle Buttons zuerst auf nicht ausgewählt
        button->setSelected(false);

        // Hebe die ausgewählten Optionen hervor
        if ((options.mode == GameMode::SOLO && button->getText() == "Spiele alleine") ||
            (options.mode == GameMode::VERSUS_AI && button->getText() == "Spiele gegen KI") ||
            (options.difficulty == AIDifficulty::EASY && button->getText() == "Einfach") ||
            (options.difficulty == AIDifficulty::MEDIUM && button->getText() == "Mittel") ||
            (options.difficulty == AIDifficulty::HARD && button->getText() == "Schwer") ||
            (options.assistant == AssistantMode::OFF && button->getText() == "Aus") ||
            (options.assistant == AssistantMode::ON && button->getText() == "An")) {
            button->setSelected(true);
        }
    }
}