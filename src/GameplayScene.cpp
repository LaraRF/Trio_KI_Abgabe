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

    // Setze die maximale Rundenanzahl aus den Optionen
    gameState.maxRounds = gameOptions.rounds;

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
    hintButton->setPosition(SCREEN_WIDTH - 120 - 20, 20 + 40 + 100);
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

    // Wenn gegen KI gespielt wird, aktualisiere die KI (nur wenn keine Runde gerade gewonnen wurde)
    if (gameOptions.mode == GameMode::VERSUS_AI && !gameState.isRoundWon() && !gameState.isGameWon()) {
        processAIMove(deltaTime);
    }

    // Wenn eine Runde gewonnen wurde und der Timer abgelaufen ist, starte eine neue Runde
    if (gameState.isRoundWon() && gameState.getRoundWonTimer() <= 0.0f && !gameState.isGameWon()) {
        // Erhöhe den Rundenzähler erst jetzt, wenn die Runde wirklich vorbei ist
        gameState.rounds++;

        if (gameOptions.mode == GameMode::VERSUS_AI) {
            // Lasse die KI aus der Spielerzeit lernen
            float playerSolveTime = gameState.getPlayerAverageTime();
            ai.learnFromPlayerMove(playerSolveTime, gameState.getWrongAttempts(), gameState.getHintsUsed());
        }

        // Setze den Timer fort, wenn eine neue Runde beginnt
        gameState.resumeTimer();

        // Starte eine neue Runde mit Zeitmessung
        gameState.startRound();

        // Starte eine neue Runde mit Zeitmessung
        gameState.startRound();

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

    // Wenn das Spiel gewonnen ist, pausiere den Timer
    if (gameState.isGameWon() && !gameState.timerPaused) {
        gameState.pauseTimer();
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

    if (gameState.isGameWon()) {
        // Wenn das Spiel zu Ende ist, aber die aktuelle Runde nicht gewonnen wurde,
        // zeichne die bisherige Zeit für den Spieler auf
        if (!gameState.isRoundWon()) {
            auto now = std::chrono::steady_clock::now();
            std::chrono::duration<float> elapsed = now - gameState.roundStartTime;
            float roundTime = elapsed.count();

            // Zeit für den Spieler aufzeichnen (da KI nicht schneller war)
            gameState.recordRoundTimeForPlayer(roundTime);
        }

        nextScene = SceneType::GAME_OVER;
        sceneFinished = true;
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

    // Zeige den Punktestand im Versus-Modus an
    if (gameOptions.mode == GameMode::VERSUS_AI) {
        char scoreText[100];
        sprintf(scoreText, "Punktestand - Spieler: %d  KI: %d",
                gameState.getPlayerScore(), gameState.getAIScore());
        DrawText(scoreText, 10, SCREEN_HEIGHT - 40, 20, BLACK);
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
        bool canSelect = false;

        if (gameState.getSelectedCells().empty()) {
            // Erste Zelle kann immer ausgewählt werden
            canSelect = true;
        } else {
            // Prüfe, ob die neue Zelle eine gültige nächste Zelle ist
            std::vector<CellPosition> validNextCells = board->getValidNextCells(gameState.getSelectedCells());

            for (const auto& validCell : validNextCells) {
                if (validCell.row == row && validCell.col == col) {
                    canSelect = true;
                    break;
                }
            }
        }

        if (canSelect) {
            // Zelle hinzufügen
            gameState.addSelectedCell(clickedCell);
            board->getCell(row, col)->setSelected(true);

            // Prüfe, ob 3 Zellen ausgewählt wurden und ob sie die Gewinnbedingung erfüllen
            if (gameState.getSelectedCells().size() == 3) {
                if (board->isValidSelection(gameState.getSelectedCells()) &&
                    board->checkWinCondition(gameState.getSelectedCells(), gameState.getTargetNumber())) {

                    // Zeit für diese Runde berechnen
                    auto now = std::chrono::steady_clock::now();
                    std::chrono::duration<float> elapsed = now - gameState.roundStartTime;
                    float roundTime = elapsed.count();

                    // Zeit für den Spieler aufzeichnen
                    gameState.recordRoundTimeForPlayer(roundTime);

                    // Punkte für den Spieler erhöhen (nur im Versus-Modus)
                    if (gameOptions.mode == GameMode::VERSUS_AI) {
                        gameState.incrementPlayerScore();
                    }

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
    // Wenn die KI bereits gewonnen hat, nichts tun
    if (gameState.isRoundWon()) return;
    ai.update(deltaTime, gameState);

    // Wenn die KI bereit ist, einen Zug zu machen
    if (ai.isReadyToMove()) {
        // KI findet eine Lösung
        std::vector<CellPosition> solution = ai.findSolution(board, gameState.getTargetNumber());

        if (!solution.empty()) {

            // Sicherstellen, dass die aktuelle Runde nicht bereits gewonnen wurde
            // (könnte passieren, wenn der Spieler genau vor der KI gewonnen hat)
            if (!gameState.isRoundWon()) {
                // Aktuelle Auswahl zurücksetzen
                for (const auto &cell: gameState.getSelectedCells()) {
                    board->getCell(cell.row, cell.col)->setSelected(false);
                }
                gameState.clearSelectedCells();

                // KI hat eine Lösung gefunden
                for (const auto &cell: solution) {
                    gameState.addSelectedCell(cell);
                    board->getCell(cell.row, cell.col)->setSelected(true);
                }
            }

            // Zeit für diese Runde berechnen
            auto now = std::chrono::steady_clock::now();
            std::chrono::duration<float> elapsed = now - gameState.roundStartTime;
            float roundTime = elapsed.count();

            // Zeit für die KI aufzeichnen
            gameState.recordRoundTimeForAI(roundTime);

            // Punkte für die KI erhöhen
            gameState.incrementAIScore();

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