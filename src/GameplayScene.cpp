#include "GameplayScene.h"

GameplayScene::GameplayScene(int boardSize, const GameOptions& options)
        : Scene(), boardSize(boardSize), board(std::make_shared<Board>(boardSize)),
          solver(board), gameOptions(options), ai(options.difficulty),
          hintLevel(0), showAssistantHint(false), aiPlayTimer(0.0f) {
}

void GameplayScene::initialize() {
    sceneFinished = false;
    nextScene = SceneType::MAIN_MENU;

    board->initialize();
    gameState.initialize();
    ai.initialize();
    hintLevel = 0;
    showAssistantHint = false;
    currentAssistantHint = "";
    aiPlayTimer = 0.0f;

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

    // Erstelle Hinweis-Button mit erweiterter Funktionalität
    auto hintButtonCallback = [this]() {
        if (!gameState.isRoundWon()) {
            if (gameOptions.assistant == AssistantMode::ON && hintLevel > 0) {
                // Bei aktivem Assistenten zeige detaillierten Hinweis
                showAssistantMessage(hintLevel);
                hintLevel = (hintLevel + 1) % 3; // Rotiere zwischen Hinweisstufen 1-2
            } else {
                // Standardverhalten: Markiere erste Zelle einer Lösung
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

                    // Aktiviere den Assistenten für den nächsten Hinweis
                    if (gameOptions.assistant == AssistantMode::ON) {
                        hintLevel = 1;
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

    // Wenn gegen KI gespielt wird, aktualisiere die KI
    if (gameOptions.mode == GameMode::VERSUS_AI && !gameState.isRoundWon()) {
        processAIMove(deltaTime);
    }

    // Wenn eine Runde gewonnen wurde und der Timer abgelaufen ist, starte eine neue Runde
    if (gameState.isRoundWon() && gameState.getRoundWonTimer() <= 0.0f && !gameState.isGameWon()) {
        if (gameOptions.mode == GameMode::VERSUS_AI) {
            // Lasse die KI aus der Spielerzeit lernen
            float playerSolveTime = gameState.getTotalTime() / std::max(1, gameState.getRounds());
            ai.learnFromPlayerMove(playerSolveTime, gameState.getWrongAttempts(), gameState.getHintsUsed());
        }

        gameState.startNewRound();

        // Alle Zellen zurücksetzen
        for (int row = 0; row < board->getSize(); row++) {
            for (int col = 0; col < board->getSize(); col++) {
                CellPtr cell = board->getCell(row, col);
                if (cell) {
                    cell->setSelected(false);
                }
            }
        }

        // Assistenten-Hinweise zurücksetzen
        hintLevel = 0;
        showAssistantHint = false;
        currentAssistantHint = "";

        generateTargetNumber();
    }

    // Prozessiere Benutzereingaben
    if (!gameState.isRoundWon()) {
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

            // Wenn kein Button geklickt wurde, prüfe Zellen-Klicks
            if (!buttonClicked) {
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
    } else {
        // Im Versus-Modus, wenn der Spieler gewonnen hat, KI informieren
        if (gameOptions.mode == GameMode::VERSUS_AI && gameState.isRoundWon()) {
            // Die KI hat verloren, also Lernaktualisierung
            float playerSolveTime = gameState.getTotalTime() / std::max(1, gameState.getRounds());
            ai.learnFromPlayerMove(playerSolveTime, gameState.getWrongAttempts(), gameState.getHintsUsed());
        }
    }

    // Prüfe, ob das Spiel gewonnen wurde
    if (gameState.isGameWon()) {
        nextScene = SceneType::GAME_OVER;
        sceneFinished = true;
    }
}

void GameplayScene::draw() const {
    // Zeichne Spielfeld
    renderer.drawBoard(board, gameState);

    // Zeichne Spielinformationen
    renderer.drawGameInfo(gameState);

    // Zeichne Assistentenhinweis, wenn aktiv
    if (showAssistantHint && !currentAssistantHint.empty()) {
        // Hintergrund für den Hinweis
        DrawRectangle(50, SCREEN_HEIGHT - 150, SCREEN_WIDTH - 100, 100, ColorAlpha(LIGHTGRAY, 0.9f));
        DrawRectangleLines(50, SCREEN_HEIGHT - 150, SCREEN_WIDTH - 100, 100, BLACK);

        // Hinweistext
        DrawText("Assistent:", 70, SCREEN_HEIGHT - 140, 20, BLACK);
        DrawText(currentAssistantHint.c_str(), 70, SCREEN_HEIGHT - 110, 18, BLACK);
    }

    // Zeichne Modi-Information
    if (gameOptions.mode == GameMode::VERSUS_AI) {
        DrawText("Spiel gegen KI", SCREEN_WIDTH - 120, 80, 15, RED);

        // Zeige Schwierigkeitsstufe
        const char* difficultyText = "";
        switch (gameOptions.difficulty) {
            case AIDifficulty::EASY: difficultyText = "Einfach"; break;
            case AIDifficulty::MEDIUM: difficultyText = "Mittel"; break;
            case AIDifficulty::HARD: difficultyText = "Schwer"; break;
        }
        DrawText(difficultyText, SCREEN_WIDTH - 120, 100, 15, RED);
    }

    if (gameOptions.assistant == AssistantMode::ON) {
        DrawText("Assistent: An", SCREEN_WIDTH - 120, 120, 15, BLUE);
    }

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

// Neue Methoden für KI und Assistent
void GameplayScene::processAIMove(float deltaTime) {
    ai.update(deltaTime, gameState);

    // Wenn die KI bereit ist, einen Zug zu machen
    if (ai.isReadyToMove()) {
        // KI findet eine Lösung
        std::vector<CellPosition> solution = ai.findSolution(board, gameState.getTargetNumber());

        if (!solution.empty()) {
            // KI hat eine Lösung gefunden und gewinnt die Runde
            for (const auto& cell : solution) {
                gameState.addSelectedCell(cell);
                board->getCell(cell.row, cell.col)->setSelected(true);
            }

            // KI gewinnt diese Runde
            gameState.winRound();

            // KI lernt aus ihrem eigenen Erfolg
            float playerAvgTime = gameState.getTotalTime() / std::max(1, gameState.getRounds());
            ai.learnFromPlayerMove(playerAvgTime, gameState.getWrongAttempts(), gameState.getHintsUsed());
        }
    }
}

void GameplayScene::showAssistantMessage(int level) {
    if (gameOptions.assistant == AssistantMode::ON) {
        // Generiere einen passenden Hinweis
        currentAssistantHint = assistant.generateHint(
                board,
                gameState.getTargetNumber(),
                gameState.getSelectedCells(),
                level - 1  // Level 0 basiert für die Assistant-Klasse
        );

        showAssistantHint = true;

        // Inkrementiere Hinweisnutzungen
        gameState.incrementHintsUsed();
    }
}