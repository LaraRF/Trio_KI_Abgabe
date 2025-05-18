//
// Created by lrfri on 18.05.2025.
//

#include "Cell.h"

Cell::Cell(int value) : GameObject(), value(value), selected(false) {}

void Cell::update(float deltaTime) {
    // Keine spezielle Aktualisierungslogik für Zellen
}

void Cell::draw() const {
    Rectangle cellRect = getBounds();

    // Zeichne Zelle mit entsprechender Farbe
    DrawRectangleRec(cellRect, selected ? SELECTED_CELL_COLOR : NORMAL_CELL_COLOR);
    DrawRectangleLinesEx(cellRect, 1, BLACK);

    // Zeichne Zahl
    char numStr[2] = { static_cast<char>('0' + value), '\0' };
    int fontSize = width * 0.5f;
    int textWidth = MeasureText(numStr, fontSize);

    DrawText(numStr,
             x + (width - textWidth) / 2,
             y + height / 2 - fontSize / 2,
             fontSize, BLACK);
}

int Cell::getValue() const {
    return value;
}

void Cell::setSelected(bool selected) {
    this->selected = selected;
}

bool Cell::isSelected() const {
    return selected;
}