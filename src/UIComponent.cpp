//
// Created by lrfri on 18.05.2025.
//

#include "UIComponent.h"

UIComponent::UIComponent() : GameObject() {}

void UIComponent::update(float deltaTime) {
    // Leere Implementierung in der Basisklasse
}

void UIComponent::draw() const {
    // Leere Implementierung in der Basisklasse
}

bool UIComponent::handleMouseClick(const Vector2& mousePosition) {
    return containsPoint(mousePosition);
}