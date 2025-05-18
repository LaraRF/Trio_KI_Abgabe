//
// Created by lrfri on 18.05.2025.
//

#ifndef RAYLIBSTARTER_UICOMPONENT_H
#define RAYLIBSTARTER_UICOMPONENT_H

#include "GameTypes.h"
#include "GameObject.h"

class UIComponent : public GameObject {
public:
    UIComponent();

    virtual void update(float deltaTime) override;
    virtual void draw() const override;

    virtual bool handleMouseClick(const Vector2& mousePosition);
};

#endif //RAYLIBSTARTER_UICOMPONENT_H
