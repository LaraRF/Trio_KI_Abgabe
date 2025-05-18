//
// Created by lrfri on 18.05.2025.
//

#ifndef RAYLIBSTARTER_BOARD_H
#define RAYLIBSTARTER_BOARD_H

#include "GameTypes.h"
#include "Cell.h"

class Board : public GameObject {
public:
    Board(int size);

    void initialize();
    void update(float deltaTime) override;
    void draw() const override;

    int getSize() const;
    int getCellValue(int row, int col) const;

    CellPtr getCell(int row, int col) const;

    bool isValidCell(int row, int col) const;

    std::vector<CellPosition> getValidNextCells(const std::vector<CellPosition>& selectedCells) const;

    bool isValidSelection(const std::vector<CellPosition>& selectedCells) const;
    bool checkWinCondition(const std::vector<CellPosition>& selectedCells, int targetNumber) const;

    void resetAllCells();
private:
    int size;
    std::vector<std::vector<CellPtr>> cells;

    bool isConsecutive(const std::vector<CellPosition>& cells) const;
};

#endif //RAYLIBSTARTER_BOARD_H
