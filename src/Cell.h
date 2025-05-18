//
// Created by lrfri on 18.05.2025.
//

#ifndef RAYLIBSTARTER_CELL_H
#define RAYLIBSTARTER_CELL_H

#include "GameTypes.h"
#include "GameObject.h"

class Cell : public GameObject {
public:
    Cell(int value);

    void update(float deltaTime) override;
    void draw() const override;

    int getValue() const;

    void setSelected(bool selected);
    bool isSelected() const;

private:
    int value;
    bool selected;
};

#endif //RAYLIBSTARTER_CELL_H
