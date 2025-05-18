//
// Created by lrfri on 18.05.2025.
//

#ifndef TRIO_KI_ABGABE_GAMETYPES_H
#define TRIO_KI_ABGABE_GAMETYPES_H

#include "raylib.h"
#include <vector>
#include <string>
#include <memory>

// Forward-Deklarationen
class GameObject;
class Board;
class Cell;
class UIComponent;
class Button;
class GameState;
class GameRenderer;
class GameSolver;
class Scene;
class MainMenuScene;
class GameplayScene;
class GameOverScene;
class TrioGame;

// Typ-Definititionen
using GameObjectPtr = std::shared_ptr<GameObject>;
using BoardPtr = std::shared_ptr<Board>;
using CellPtr = std::shared_ptr<Cell>;
using UIComponentPtr = std::shared_ptr<UIComponent>;
using ButtonPtr = std::shared_ptr<Button>;
using ScenePtr = std::shared_ptr<Scene>;

// Spielzustände
enum class SceneType {
    MAIN_MENU,
    OPTIONS,
    GAMEPLAY,
    GAME_OVER
};

// Struktur für eine ausgewählte Zelle
struct CellPosition {
    int row;
    int col;

    bool operator==(const CellPosition& other) const {
        return row == other.row && col == other.col;
    }

    bool operator!=(const CellPosition& other) const {
        return !(*this == other);
    }
};

// Konstanten
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const char* WINDOW_TITLE;

// Farbkonstanten
extern const Color NORMAL_CELL_COLOR;
extern const Color SELECTED_CELL_COLOR;
extern const Color BUTTON_COLOR;
extern const Color BUTTON_HOVER_COLOR;
extern const Color BUTTON_TEXT_COLOR;

enum class GameMode {
    SOLO,
    VERSUS_AI
};

enum class AIDifficulty {
    EASY,
    MEDIUM,
    HARD
};

enum class AssistantMode {
    OFF,
    ON
};

struct GameOptions {
    GameMode mode = GameMode::SOLO;
    AIDifficulty difficulty = AIDifficulty::MEDIUM;
    AssistantMode assistant = AssistantMode::OFF;
};

#endif //TRIO_KI_ABGABE_GAMETYPES_H
