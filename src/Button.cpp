//
// Created by lrfri on 18.05.2025.
//

#include "Button.h"

Button::Button(const std::string& text, std::function<void()> callback)
        : UIComponent(), text(text), callback(callback), hover(false) {}

void Button::update(float deltaTime) {
    Vector2 mousePos = GetMousePosition();
    hover = containsPoint(mousePos);
}

void Button::draw() const {
    Rectangle buttonRect = getBounds();

    DrawRectangleRec(buttonRect, hover ? BUTTON_HOVER_COLOR : BUTTON_COLOR);
    DrawRectangleLinesEx(buttonRect, 2, BLACK);

    int textWidth = MeasureText(text.c_str(), 20);
    DrawText(text.c_str(),
             x + (width - textWidth) / 2,
             y + height / 2 - 10,
             20, BUTTON_TEXT_COLOR);
}

bool Button::handleMouseClick(const Vector2& mousePosition) {
    if (UIComponent::handleMouseClick(mousePosition)) {
        if (callback) {
            callback();
        }
        return true;
    }
    return false;
}

void Button::setText(const std::string& text) {
    this->text = text;
}

std::string Button::getText() const {
    return text;
}
