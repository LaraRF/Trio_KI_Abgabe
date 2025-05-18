//
// Created by lrfri on 18.05.2025.
//

#include "OptionsScene.h"
#include <string>

OptionsScene::OptionsScene(const GameOptions& currentOptions)
        : Scene(), options(currentOptions), roundsValue(currentOptions.rounds) {
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

        Rectangle minusRect = { 400 + 200 + 10, 450, 40, 40 };
        Rectangle plusRect = { minusRect.x + 50, 450, 40, 40 };

        if (CheckCollisionPointRec(mousePos, minusRect)) {
            // Minus-Button
            roundsValue = std::max(1, roundsValue - 1);
            options.rounds = roundsValue;
        }
        else if (CheckCollisionPointRec(mousePos, plusRect)) {
            // Plus-Button
            roundsValue = std::min(50, roundsValue + 1);
            options.rounds = roundsValue;
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

    // Kategorie "Rundenanzahl" hinzufügen
            DrawText("Rundenanzahl:", 100, 450, 30, BLACK);

    // Anzeige der Rundenanzahl
    DrawText(TextFormat("Aktiv: %d Runden", options.rounds), 100, 490, 20, DARKBLUE);

    // Zeichne einen Bereich für die Rundeneinstellung
    Rectangle roundsRect = { 400, 450, 200, 40 };
    DrawRectangleRec(roundsRect, LIGHTGRAY);
    DrawRectangleLinesEx(roundsRect, 2, BLACK);

    // Anzeige der aktuellen Rundenanzahl
    char roundsText[10];
    sprintf(roundsText, "%d", roundsValue);
    int textWidth = MeasureText(roundsText, 30);
    DrawText(roundsText, roundsRect.x + (roundsRect.width - textWidth) / 2,
             roundsRect.y + 5, 30, BLACK);

    // Zeichne Plus- und Minus-Buttons
    Rectangle minusRect = { roundsRect.x + roundsRect.width + 10, roundsRect.y, 40, 40 };
    Rectangle plusRect = { minusRect.x + 50, roundsRect.y, 40, 40 };

    DrawRectangleRec(minusRect, LIGHTGRAY);
    DrawRectangleLinesEx(minusRect, 2, BLACK);
    DrawText("-", minusRect.x + 15, minusRect.y + 5, 30, BLACK);

    DrawRectangleRec(plusRect, LIGHTGRAY);
    DrawRectangleLinesEx(plusRect, 2, BLACK);
    DrawText("+", plusRect.x + 15, plusRect.y + 5, 30, BLACK);
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