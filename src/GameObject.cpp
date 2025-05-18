//
// Created by lrfri on 18.05.2025.
//

#include "GameObject.h"

GameObject::GameObject() : x(0), y(0), width(0), height(0) {}

void GameObject::update(float deltaTime) {
    // Leere Basisimplementierung
}

void GameObject::draw() const {
    // Leere Basisimplementierung
}

void GameObject::setPosition(float x, float y) {
    this->x = x;
    this->y = y;
}

void GameObject::setSize(float width, float height) {
    this->width = width;
    this->height = height;
}

float GameObject::getX() const {
    return x;
}

float GameObject::getY() const {
    return y;
}

float GameObject::getWidth() const {
    return width;
}

float GameObject::getHeight() const {
    return height;
}

Rectangle GameObject::getBounds() const {
    return { x, y, width, height };
}

bool GameObject::containsPoint(const Vector2& point) const {
    return CheckCollisionPointRec(point, getBounds());
}