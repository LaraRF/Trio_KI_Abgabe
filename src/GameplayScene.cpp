#include "GameplayScene.h"

GameplayScene::GameplayScene(int boardSize)
        : Scene(), boardSize(boardSize), board(std::make_shared<Board>(boardSize)), solver(board) {}

void GameplayScene::initialize() {
    sceneFinished = false;
    nextScene = SceneType::MAIN_MENU;

    board->initialize();
    gameState.initialize();

    buttons.clear();

    // Erstelle Zurück-Button
    auto backButtonCallback = [this]() {
        nextScene = SceneType::MAIN_MENU;
        sceneFinished = true;
    };

    ButtonPtr backButton = std::make_shared<Button>("Zurück", backButtonCallback);
    backButton->setPosition(SCREEN_WIDTH - 120 - 20, 20);
    backButton->setSize(120, 40);
    buttons.push_back(backButton);

    // Erstelle Hinweis-Button
    auto hintButtonCallback = [this]() {
        if (!gameState.isRoundWon()) {
            std::vector<CellPosition> hintCells = solver.getHint(gameState.getTargetNumber());

            if (!hintCells.empty()) {
                gameState.clearSelectedCells();
                gameState.addSelectedCell(hintCells[0]);
                gameState.incrementHintsUsed();

                // Markiere die Zelle auf dem Spielfeld
                for (int row = 0; row < board->getSize(); row++) {
                    for (int col = 0; col < board->getSize(); col++) {
                        CellPtr cell = board->getCell(row, col);
                        cell->setSelected(gameState.isCellSelected({row, col}));
                    }
                }
            }
        }
    };

    ButtonPtr hintButton = std::make_shared<Button>("Hinweis", hintButtonCallback);
    hintButton->setPosition(SCREEN_WIDTH - 120 - 20, 20 + 40 + 10);
    hintButton->setSize(120, 40);
    buttons.push_back(hintButton);

    // Generiere erste Zielzahl
    generateTargetNumber();
}

void GameplayScene::update(float deltaTime) {
    gameState.update(deltaTime);

    // Update Board
    board->update(deltaTime);

    // Update Buttons
    for (auto& button : buttons) {
        button->update(deltaTime);
    }

    // Wenn eine Runde gewonnen wurde und der Timer abgelaufen ist, starte eine neue Runde
    if (gameState.isRoundWon() && gameState.getRoundWonTimer() <= 0.0f && !gameState.isGameWon()) {
        gameState.startNewRound();
        board->resetAllCells();
        generateTargetNumber();
    }

    // Prüfe, ob das Spiel gewonnen wurde
    if (gameState.isGameWon()) {
        nextScene = SceneType::GAME_OVER;
        sceneFinished = true;
    }

    // Prozessiere Benutzereingaben
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();

        // Prüfe Button-Klicks
        bool buttonClicked = false;
        for (auto& button : buttons) {
            if (button->handleMouseClick(mousePos)) {
                buttonClicked = true;
                break;
            }
        }

        // Wenn kein Button geklickt wurde und die Runde nicht bereits gewonnen ist,
        // prüfe, ob eine Zelle angeklickt wurde
        if (!buttonClicked && !gameState.isRoundWon()) {
            for (int row = 0; row < board->getSize(); row++) {
                for (int col = 0; col < board->getSize(); col++) {
                    CellPtr cell = board->getCell(row, col);
                    if (cell && cell->containsPoint(mousePos)) {
                        processClick(row, col);
                        break;
                    }
                }
            }
        }
    }
}

void GameplayScene::draw() const {
    // Zeichne Spielfeld
    renderer.drawBoard(board, gameState);

    // Zeichne Spielinformationen
    renderer.drawGameInfo(gameState);

    // Zeichne Buttons
    for (const auto& button : buttons) {
        button->draw();
    }
}

SceneType GameplayScene::getNextScene() const {
    return nextScene;
}

bool GameplayScene::isSceneFinished() const {
    return sceneFinished;
}

const GameState& GameplayScene::getGameState() const {
    return gameState;
}

void GameplayScene::generateTargetNumber() {
    int targetNumber;
    std::vector<int> usedNumbers;

    for (int i = 0; i < gameState.getRounds(); i++) {
        usedNumbers.push_back(gameState.getTargetNumber());
    }

    if (solver.generateTargetNumber(targetNumber, usedNumbers)) {
        gameState.setTargetNumber(targetNumber);
        gameState.addUsedTargetNumber(targetNumber);
    } else {
        // Notfall: Verwende eine einfache Zahl
        gameState.setTargetNumber(12);
    }
}

void GameplayScene::processClick(int row, int col) {
    CellPosition clickedCell = {row, col};

    // Prüfe, ob die Zelle bereits ausgewählt ist
    if (gameState.isCellSelected(clickedCell)) {
        // Zelle abwählen
        gameState.removeSelectedCell(clickedCell);
        board->getCell(row, col)->setSelected(false);
    } else {
        // Prüfe, ob die neue Zelle ausgewählt werden kann
        if (gameState.getSelectedCells().empty() ||
            board->getValidNextCells(gameState.getSelectedCells()).end() !=
            std::find_if(board->getValidNextCells(gameState.getSelectedCells()).begin(),
                         board->getValidNextCells(gameState.getSelectedCells()).end(),
                         [&clickedCell](const CellPosition& pos) {
                             return pos.row == clickedCell.row && pos.col == clickedCell.col;
                         })) {

            // Zelle hinzufügen
            gameState.addSelectedCell(clickedCell);
            board->getCell(row, col)->setSelected(true);

            // Prüfe, ob 3 Zellen ausgewählt wurden und ob sie die Gewinnbedingung erfüllen
            if (gameState.getSelectedCells().size() == 3) {
                if (board->isValidSelection(gameState.getSelectedCells()) &&
                    board->checkWinCondition(gameState.getSelectedCells(), gameState.getTargetNumber())) {
                    // Runde gewonnen
                    gameState.winRound();
                } else {
                    // Ungültige Kombination, Auswahl zurücksetzen
                    for (const auto& cell : gameState.getSelectedCells()) {
                        board->getCell(cell.row, cell.col)->setSelected(false);
                    }
                    gameState.clearSelectedCells();
                    gameState.incrementWrongAttempts();
                }
            }
        }
    }
}