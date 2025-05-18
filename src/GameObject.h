//
// Created by lrfri on 18.05.2025.
//

#ifndef RAYLIBSTARTER_GAMEOBJECT_H
#define RAYLIBSTARTER_GAMEOBJECT_H

#include "GameTypes.h"

class GameObject {
public:
    GameObject();
    virtual ~GameObject() = default;

    virtual void update(float deltaTime);
    virtual void draw() const;

    void setPosition(float x, float y);
    void setSize(float width, float height);

    float getX() const;
    float getY() const;
    float getWidth() const;
    float getHeight() const;

    Rectangle getBounds() const;

    bool containsPoint(const Vector2& point) const;

protected:
    float x;
    float y;
    float width;
    float height;
};

#endif //RAYLIBSTARTER_GAMEOBJECT_H
