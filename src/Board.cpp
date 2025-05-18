//
// Created by lrfri on 18.05.2025.
//

#include "Board.h"
#include <random>
#include <algorithm>
#include <cmath>

Board::Board(int size) : GameObject(), size(size) {
    cells.resize(size, std::vector<CellPtr>(size));
}

void Board::initialize() {
    // Liste für alle Zahlen gemäß der Häufigkeitsverteilung erstellen
    std::vector<int> allNumbers;

    // Je nach Spielfeldgröße die richtige Verteilung verwenden
    if (size == 5) {
        // 5x5 Verteilung
        for (int i = 1; i <= 9; i++) {
            int count = 0;
            switch (i) {
                case 1: case 2: case 7: case 8: case 9: count = 2; break;
                case 3: case 6: count = 3; break;
                case 4: case 5: count = 4; break;
            }
            allNumbers.insert(allNumbers.end(), count, i);
        }
    }
    else if (size == 7) {
        // 7x7 Verteilung
        for (int i = 1; i <= 9; i++) {
            int count = 0;
            switch (i) {
                case 1: case 7: case 8: case 9: count = 5; break;
                case 2: case 3: case 4: case 5: case 6: count = 6; break;
            }
            allNumbers.insert(allNumbers.end(), count, i);
        }
    }
    else if (size == 10) {
        // 10x10 Verteilung
        for (int i = 1; i <= 9; i++) {
            int count = 0;
            switch (i) {
                case 1: case 9: count = 12; break;
                case 2: case 3: case 4: case 5: case 6: count = 12; break;
                case 7: case 8: count = 10; break;
            }
            allNumbers.insert(allNumbers.end(), count, i);
        }
    }

    // Überprüfung, ob die richtige Anzahl Zahlen generiert wurde
    int totalCells = size * size;
    if (allNumbers.size() != totalCells) {
        // Wenn nicht genug Zahlen, füge weitere hinzu
        while (allNumbers.size() < totalCells) {
            allNumbers.push_back(1 + rand() % 9);
        }
        // Wenn zu viele Zahlen, entferne überschüssige
        while (allNumbers.size() > totalCells) {
            allNumbers.pop_back();
        }
    }

    // Mische die Zahlen
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(allNumbers.begin(), allNumbers.end(), gen);

    // Berechne Zellengröße und Startposition
    float cellSize = std::min((float)(SCREEN_WIDTH - 100) / size,
                              (float)(SCREEN_HEIGHT - 100) / size);

    float startX = (SCREEN_WIDTH - cellSize * size) / 2;
    float startY = (SCREEN_HEIGHT - cellSize * size) / 2;

    // Erstelle Zellen
    int index = 0;
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            cells[row][col] = std::make_shared<Cell>(allNumbers[index++]);
            cells[row][col]->setPosition(startX + col * cellSize, startY + row * cellSize);
            cells[row][col]->setSize(cellSize, cellSize);
        }
    }
}

void Board::update(float deltaTime) {
    for (auto& row : cells) {
        for (auto& cell : row) {
            cell->update(deltaTime);
        }
    }
}

void Board::draw() const {
    for (const auto& row : cells) {
        for (const auto& cell : row) {
            cell->draw();
        }
    }
}

int Board::getSize() const {
    return size;
}

int Board::getCellValue(int row, int col) const {
    if (isValidCell(row, col)) {
        return cells[row][col]->getValue();
    }
    return -1;
}

CellPtr Board::getCell(int row, int col) const {
    if (isValidCell(row, col)) {
        return cells[row][col];
    }
    return nullptr;
}

bool Board::isValidCell(int row, int col) const {
    return row >= 0 && row < size && col >= 0 && col < size;
}

std::vector<CellPosition> Board::getValidNextCells(const std::vector<CellPosition>& selectedCells) const {
    std::vector<CellPosition> validCells;

    if (selectedCells.empty()) {
        // Wenn noch keine Zelle ausgewählt ist, können alle Zellen gewählt werden
        for (int row = 0; row < size; row++) {
            for (int col = 0; col < size; col++) {
                validCells.push_back({row, col});
            }
        }
        return validCells;
    }

    if (selectedCells.size() >= 3) {
        // Maximal 3 Zellen können ausgewählt werden
        return validCells;
    }

    // Wenn bereits eine Zelle ausgewählt ist, muss die neue Zelle benachbart sein
    if (selectedCells.size() == 1) {
        int row0 = selectedCells[0].row;
        int col0 = selectedCells[0].col;

        // Alle 8 möglichen Nachbarn prüfen
        for (int r = -1; r <= 1; r++) {
            for (int c = -1; c <= 1; c++) {
                if (r == 0 && c == 0) continue;  // Nicht die Zelle selbst

                int newRow = row0 + r;
                int newCol = col0 + c;

                if (isValidCell(newRow, newCol)) {
                    validCells.push_back({newRow, newCol});
                }
            }
        }
    }

    // Wenn bereits zwei Zellen ausgewählt sind, muss die neue Zelle in der gleichen Linie liegen
    if (selectedCells.size() == 2) {
        int row0 = selectedCells[0].row;
        int col0 = selectedCells[0].col;
        int row1 = selectedCells[1].row;
        int col1 = selectedCells[1].col;

        // Horizontal
        if (row0 == row1) {
            int col = 2 * col1 - col0;  // Nächste Zelle in der gleichen Richtung
            if (isValidCell(row0, col)) {
                validCells.push_back({row0, col});
            }

            col = 2 * col0 - col1;  // Nächste Zelle in der entgegengesetzten Richtung
            if (isValidCell(row0, col)) {
                validCells.push_back({row0, col});
            }
        }
            // Vertikal
        else if (col0 == col1) {
            int row = 2 * row1 - row0;  // Nächste Zelle in der gleichen Richtung
            if (isValidCell(row, col0)) {
                validCells.push_back({row, col0});
            }

            row = 2 * row0 - row1;  // Nächste Zelle in der entgegengesetzten Richtung
            if (isValidCell(row, col0)) {
                validCells.push_back({row, col0});
            }
        }
            // Diagonal
        else if (std::abs(row1 - row0) == 1 && std::abs(col1 - col0) == 1) {
            int rowDiff = row1 - row0;
            int colDiff = col1 - col0;

            // Nächste Zelle in der gleichen Richtung
            int row = row1 + rowDiff;
            int col = col1 + colDiff;
            if (isValidCell(row, col)) {
                validCells.push_back({row, col});
            }

            // Nächste Zelle in der entgegengesetzten Richtung
            row = row0 - rowDiff;
            col = col0 - colDiff;
            if (isValidCell(row, col)) {
                validCells.push_back({row, col});
            }
        }
    }

    return validCells;
}

bool Board::isValidSelection(const std::vector<CellPosition>& selectedCells) const {
    if (selectedCells.size() != 3) return false;

    // Prüfe, ob die Zellen in einer horizontalen, vertikalen oder diagonalen Linie liegen
    int row0 = selectedCells[0].row;
    int col0 = selectedCells[0].col;
    int row1 = selectedCells[1].row;
    int col1 = selectedCells[1].col;
    int row2 = selectedCells[2].row;
    int col2 = selectedCells[2].col;

    // Prüfe horizontal
    bool isHorizontal = (row0 == row1 && row1 == row2);

    // Prüfe vertikal
    bool isVertical = (col0 == col1 && col1 == col2);

    // Prüfe diagonal (beide Richtungen)
    bool isDiagonal = (std::abs(row0 - row1) == std::abs(col0 - col1) &&
                       std::abs(row1 - row2) == std::abs(col1 - col2) &&
                       std::abs(row0 - row2) == std::abs(col0 - col2));

    // Prüfe, ob die Zellen nebeneinander liegen
    bool isConsecutive = this->isConsecutive(selectedCells);

    return (isHorizontal || isVertical || isDiagonal) && isConsecutive;
}

bool Board::isConsecutive(const std::vector<CellPosition>& positions) const {
    if (positions.size() != 3) return false;

    int row0 = positions[0].row;
    int col0 = positions[0].col;
    int row1 = positions[1].row;
    int col1 = positions[1].col;
    int row2 = positions[2].row;
    int col2 = positions[2].col;

    // Prüfe horizontal
    if (row0 == row1 && row1 == row2) {
        // Sortiere die Zellen nach Spalte
        std::vector<int> cols = {col0, col1, col2};
        std::sort(cols.begin(), cols.end());
        return (cols[1] - cols[0] == 1 && cols[2] - cols[1] == 1);
    }

    // Prüfe vertikal
    if (col0 == col1 && col1 == col2) {
        // Sortiere die Zellen nach Zeile
        std::vector<int> rows = {row0, row1, row2};
        std::sort(rows.begin(), rows.end());
        return (rows[1] - rows[0] == 1 && rows[2] - rows[1] == 1);
    }

    // Prüfe diagonal
    if (std::abs(row0 - row1) == std::abs(col0 - col1) &&
        std::abs(row1 - row2) == std::abs(col1 - col2) &&
        std::abs(row0 - row2) == std::abs(col0 - col2)) {

        // Für diagonale Verbindungen müssen wir die Zellen sortieren
        // und dann prüfen, ob sie aufeinanderfolgen
        std::vector<std::pair<int, int>> positions = {
                {row0, col0}, {row1, col1}, {row2, col2}
        };

        // Sortiere nach Zeile und dann nach Spalte
        std::sort(positions.begin(), positions.end());

        // Prüfe, ob die Zellen diagonal nebeneinander liegen
        return (positions[1].first - positions[0].first == 1 &&
                positions[2].first - positions[1].first == 1 &&
                std::abs(positions[1].second - positions[0].second) == 1 &&
                std::abs(positions[2].second - positions[1].second) == 1);
    }

    return false;
}

bool Board::checkWinCondition(const std::vector<CellPosition>& selectedCells, int targetNumber) const {
    if (selectedCells.size() != 3) return false;

    // Hole die Werte der ausgewählten Zellen in der Reihenfolge, wie sie ausgewählt wurden
    int val0 = getCellValue(selectedCells[0].row, selectedCells[0].col);
    int val1 = getCellValue(selectedCells[1].row, selectedCells[1].col);
    int val2 = getCellValue(selectedCells[2].row, selectedCells[2].col);

    // Nur diese zwei Fälle prüfen (immer die beiden ersten Zahlen multiplizieren)
    bool check1 = (val0 * val1 + val2 == targetNumber);
    bool check2 = (val0 * val1 - val2 == targetNumber);

    return check1 || check2;
}

void Board::resetAllCells() {
    for (auto& row : cells) {
        for (auto& cell : row) {
            if (cell) {
                cell->setSelected(false);
            }
        }
    }
}
