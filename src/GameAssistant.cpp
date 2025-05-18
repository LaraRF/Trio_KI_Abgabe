//
// Created by lrfri on 18.05.2025.
//

#include "GameAssistant.h"
#include <random>
#include <sstream>

GameAssistant::GameAssistant() {
    // Initialisiere grundlegende Hinweise
    basicHints = {
            "Suche nach Zahlen, die beim Multiplizieren ein Ergebnis nahe der Zielzahl ergeben.",
            "Vergiss nicht, dass du auch subtrahieren kannst, nicht nur addieren!",
            "Versuche, mit den größeren Zahlen auf dem Feld zu beginnen.",
            "Manchmal ist es einfacher, von der Zielzahl aus rückwärts zu denken.",
            "Achte auf diagonale Reihen, die werden oft übersehen!"
    };

    // Initialisiere erweiterte Hinweise
    advancedHints = {
            "Suche nach zwei Zahlen, deren Produkt knapp unter oder über der Zielzahl liegt.",
            "Für die Zielzahl {target}, suche zwei Zahlen, deren Produkt etwa {target-5} bis {target+5} ist.",
            "Wenn du die beiden Zahlen x und y multiplizierst, brauchst du eine dritte Zahl z, sodass x*y+z oder x*y-z die Zielzahl ergibt.",
            "Versuche Zahlenpaare zu finden, deren Produkt dividiert durch eine dritte Zahl die Zielzahl ergibt.",
            "Achte besonders auf die Kombination von niedrigen und hohen Zahlen, oft ergibt 9*2+3 eine bessere Kombination als 5*5+0."
    };
}

std::string GameAssistant::generateHint(const BoardPtr& board, int targetNumber, const std::vector<CellPosition>& selectedCells, int hintLevel) {
    if (hintLevel == 0) {
        // Zufälligen grundlegenden Hinweis auswählen
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, basicHints.size() - 1);
        std::string hint = basicHints[dis(gen)];

        // Ersetze Platzhalter
        size_t pos = hint.find("{target}");
        while (pos != std::string::npos) {
            hint.replace(pos, 8, std::to_string(targetNumber));
            pos = hint.find("{target}", pos + 1);
        }

        return hint;
    } else {
        // Generiere einen spezifischeren Hinweis basierend auf dem aktuellen Spielzustand
        return analyzeGameState(board, targetNumber, selectedCells);
    }
}

std::string GameAssistant::analyzeGameState(const BoardPtr& board, int targetNumber, const std::vector<CellPosition>& selectedCells) {
    std::stringstream hint;

    // Fall 1: Keine Zellen ausgewählt
    if (selectedCells.empty()) {
        // Finde eine mögliche Lösung und gib einen Hinweis auf die erste Zelle
        std::vector<CellPosition> solution = findPartialSolution(board, targetNumber, {});

        if (!solution.empty()) {
            hint << "Schau dir die Zahl " << board->getCellValue(solution[0].row, solution[0].col)
                 << " " << describeLocation(solution[0], board) << " an. "
                 << "Diese könnte ein guter Startpunkt sein, um die Zielzahl " << targetNumber << " zu erreichen.";
        } else {
            hint << "Für die Zielzahl " << targetNumber << ", suche nach zwei Zahlen, "
                 << "deren Produkt nahe bei " << targetNumber << " liegt, sodass du mit der dritten Zahl "
                 << "durch Addition oder Subtraktion genau " << targetNumber << " erreichst.";
        }
    }
        // Fall 2: Eine Zelle ausgewählt
    else if (selectedCells.size() == 1) {
        int val1 = board->getCellValue(selectedCells[0].row, selectedCells[0].col);

        // Finde eine mögliche Lösung, die mit der ausgewählten Zelle beginnt
        std::vector<CellPosition> solution = findPartialSolution(board, targetNumber, selectedCells);

        if (solution.size() >= 2) {
            int val2 = board->getCellValue(solution[1].row, solution[1].col);
            hint << "Die Zahl " << val1 << ", die du ausgewählt hast, "
                 << "könnte gut mit der Zahl " << val2 << " " << describeLocation(solution[1], board)
                 << " kombiniert werden. " << val1 << " * " << val2 << " = " << (val1 * val2)
                 << ", was " << std::abs(targetNumber - (val1 * val2))
                 << " von deiner Zielzahl " << targetNumber << " entfernt ist.";
        } else {
            hint << "Mit der Zahl " << val1 << " brauchst du eine zweite Zahl, die beim Multiplizieren "
                 << "ein Ergebnis nahe bei " << targetNumber << " ergibt. "
                 << "Schau nach Zahlen, die etwa " << (targetNumber / val1) << " sind.";
        }
    }
        // Fall 3: Zwei Zellen ausgewählt
    else if (selectedCells.size() == 2) {
        int val1 = board->getCellValue(selectedCells[0].row, selectedCells[0].col);
        int val2 = board->getCellValue(selectedCells[1].row, selectedCells[1].col);
        int product = val1 * val2;

        // Berechne, welche dritte Zahl benötigt wird
        int neededAddition = targetNumber - product;
        int neededSubtraction = product - targetNumber;

        hint << "Du hast " << val1 << " und " << val2 << " ausgewählt, deren Produkt " << product << " ist. ";

        if (neededAddition > 0 && neededAddition <= 9) {
            hint << "Du brauchst eine " << neededAddition << " als dritte Zahl für Addition.";
        } else if (neededSubtraction > 0 && neededSubtraction <= 9) {
            hint << "Du brauchst eine " << neededSubtraction << " als dritte Zahl für Subtraktion.";
        } else {
            hint << "Diese Kombination führt nicht direkt zur Zielzahl " << targetNumber << ". "
                 << "Versuche eine andere Kombination, deren Produkt näher an " << targetNumber << " liegt.";
        }
    }

    return hint.str();
}

std::vector<CellPosition> GameAssistant::findPartialSolution(const BoardPtr& board, int targetNumber, const std::vector<CellPosition>& selectedCells) {
    // Wenn bereits alle 3 Zellen ausgewählt wurden, prüfe, ob sie eine Lösung bilden
    if (selectedCells.size() == 3) {
        if (board->checkWinCondition(selectedCells, targetNumber)) {
            return selectedCells;
        }
        return {};
    }

    // Startpunkt der Suche
    std::vector<CellPosition> currentPath = selectedCells;

    // Erhalte die nächsten möglichen Zellen
    std::vector<CellPosition> nextCells;
    if (currentPath.empty()) {
        // Wenn noch keine Zelle ausgewählt ist, können alle Zellen gewählt werden
        for (int row = 0; row < board->getSize(); row++) {
            for (int col = 0; col < board->getSize(); col++) {
                nextCells.push_back({row, col});
            }
        }
    } else {
        // Andernfalls nur die gültigen Nachbarzellen
        nextCells = board->getValidNextCells(currentPath);
    }

    // Durchsuche alle möglichen nächsten Zellen
    for (const auto& cell : nextCells) {
        currentPath.push_back(cell);
        auto solution = findPartialSolution(board, targetNumber, currentPath);
        if (!solution.empty()) {
            return solution;
        }
        currentPath.pop_back();
    }

    return {};
}

std::string GameAssistant::describeLocation(const CellPosition& position, const BoardPtr& board) {
    // Erstelle eine benutzerfreundliche Beschreibung der Position
    int row = position.row;
    int col = position.col;
    int size = board->getSize();

    std::stringstream description;

    // Horizontale Position
    if (col < size / 3) {
        description << "links";
    } else if (col >= 2 * size / 3) {
        description << "rechts";
    } else {
        description << "in der Mitte";
    }

    // Vertikale Position
    if (row < size / 3) {
        description << " oben";
    } else if (row >= 2 * size / 3) {
        description << " unten";
    } else {
        description << " mittig";
    }

    return description.str();
}